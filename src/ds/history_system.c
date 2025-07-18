/*======================================================================*
 *  src/ds/history_system.c
 *  Undo / Redo history system (opaque, allocator-aware, non-thread-safe)
 *  ガイドライン 2025-07 完全対応：警告ゼロ・DI・ABI安全・所有権明示
 *======================================================================*/
#include "ds/history_system.h"
#include "util/memory.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*──────────────────── 内部ノード型 ────────────────────*/
typedef struct history_node {
    ds_command_t        *cmd;     // 外部所有: コマンド本体は呼び出し側管理
    struct history_node *prev;
    struct history_node *next;
} history_node_t;

/*──────────────────── Opaque本体 ────────────────────*/
struct ds_history_system {
    const ds_allocator_t *alloc;  // NULL許可: デフォルトにフォールバック
    size_t                max_hist;
    history_node_t       *head;   // 先頭（最古）
    history_node_t       *tail;   // 末尾（最新／MRU）
    history_node_t       *curr;   // 現在位置（Undoポインタ）
    ds_stats_t            stats;  // 統計情報（将来のために保持）
};

/*──────────────────── 内部 util ────────────────────*/
/* 汎用 xmalloc/xfree：alloc優先（テスト時DI/本番はNULLでmalloc/calloc） */
static void *xmalloc(const ds_allocator_t *a, size_t n, size_t sz)
{ return a ? a->alloc(n, sz) : calloc(n, sz); }

static void xfree(const ds_allocator_t *a, void *p)
{ if (p) (a ? a->free(p) : free(p)); }

/* 全履歴ノード解放＋ポインタ初期化。alloc責任を明示。*/
static void clear_all(const ds_allocator_t *a, ds_history_system_t *h)
{
    for (history_node_t *p = h->head; p; ) {
        history_node_t *nxt = p->next;
        xfree(a, p);
        p = nxt;
    }
    h->head = h->tail = h->curr = NULL;
    h->stats.total_elements = 0;
    h->stats.operations_count++;  // クリア操作も記録
    (void)h->stats.operations_count; // unused-but-set防止（CI警告ゼロ）
}

/*======================================================================*
 *  Public API implementation
 *======================================================================*/
ds_error_t
ds_history_system_create(const ds_allocator_t *alloc,
                         size_t                max_hist,
                         ds_history_system_t **out_history)
{
    if (!out_history) return DS_ERR_NULL_POINTER;
    if (max_hist == 0) max_hist = SIZE_MAX;

    ds_history_system_t *h = xmalloc(alloc, 1, sizeof *h);
    if (!h) return DS_ERR_ALLOC;

    memset(h, 0, sizeof *h);
    h->alloc    = alloc;
    h->max_hist = max_hist;
    h->stats.creation_timestamp = (size_t)time(NULL);
    h->stats.total_elements = 0;
    h->stats.operations_count = 0;

    *out_history = h;
    return DS_SUCCESS;
}

ds_error_t
ds_history_system_destroy(const ds_allocator_t *alloc,
                          ds_history_system_t  *hist)
{
    if (!hist) return DS_ERR_NULL_POINTER;
    /* 最優先：明示的にalloc→hist->alloc優先、必ず全ノード→本体free */
    const ds_allocator_t *a = alloc ? alloc : hist->alloc;
    clear_all(a, hist);
    xfree(a, hist);
    return DS_SUCCESS;
}

ds_error_t
ds_history_system_execute_command(const ds_allocator_t *alloc,
                                  ds_history_system_t  *hist,
                                  const ds_command_t   *command)
{
    (void)alloc; // 現状はhist->allocのみ使う。DI将来拡張用
    if (!hist || !command)         return DS_ERR_NULL_POINTER;
    if (!command->execute)         return DS_ERR_INVALID_ARG;

    ds_error_t rc = command->execute(command->ctx);
    if (rc != DS_SUCCESS)          return rc;

    // 新ノード作成
    history_node_t *node = xmalloc(hist->alloc, 1, sizeof *node);
    if (!node) return DS_ERR_ALLOC;
    node->cmd  = (ds_command_t *)command;
    node->prev = hist->tail;
    node->next = NULL;

    // MRU挿入
    if (hist->tail) hist->tail->next = node;
    else            hist->head       = node;
    hist->tail  = node;
    hist->curr  = node;

    // 履歴数カウント・統計反映
    size_t count = 0;
    for (history_node_t *p = hist->tail; p; p = p->prev) ++count;
    hist->stats.total_elements = count;
    hist->stats.operations_count++;
    (void)hist->stats.operations_count; // unused-but-set防止

    // 容量超過ならFIFO削除（最古ノードだけfree。他ノード影響なし）
    if (hist->max_hist != SIZE_MAX && count > hist->max_hist) {
        history_node_t *victim = hist->head;
        hist->head = victim->next;
        if (hist->head) hist->head->prev = NULL;
        xfree(hist->alloc, victim);
        hist->stats.total_elements--;
    }
    return DS_SUCCESS;
}

bool ds_history_system_can_undo(const ds_history_system_t *hist)
{ return hist && hist->curr; }

bool ds_history_system_can_redo(const ds_history_system_t *hist)
{ return hist && hist->curr && hist->curr->next; }

ds_error_t
ds_history_system_undo(const ds_allocator_t *alloc,
                       ds_history_system_t  *hist)
{
    (void)alloc;
    if (!hist)                 return DS_ERR_NULL_POINTER;
    if (!hist->curr)           return DS_ERR_EMPTY;
    if (!hist->curr->cmd->undo) return DS_ERR_INVALID_ARG;

    ds_error_t rc = hist->curr->cmd->undo(hist->curr->cmd->ctx);
    if (rc == DS_SUCCESS) {
        hist->curr = hist->curr->prev;
        hist->stats.operations_count++;
        (void)hist->stats.operations_count;
    }
    return rc;
}

ds_error_t
ds_history_system_redo(const ds_allocator_t *alloc,
                       ds_history_system_t  *hist)
{
    (void)alloc;
    if (!hist)                             return DS_ERR_NULL_POINTER;
    if (!hist->curr || !hist->curr->next)  return DS_ERR_EMPTY;

    history_node_t *next = hist->curr->next;
    if (!next->cmd->execute)               return DS_ERR_INVALID_ARG;

    ds_error_t rc = next->cmd->execute(next->cmd->ctx);
    if (rc == DS_SUCCESS) {
        hist->curr = next;
        hist->stats.operations_count++;
        (void)hist->stats.operations_count;
    }
    return rc;
}

ds_error_t
ds_history_system_clear(const ds_allocator_t *alloc,
                        ds_history_system_t  *hist)
{
    if (!hist) return DS_ERR_NULL_POINTER;
    const ds_allocator_t *a = alloc ? alloc : hist->alloc;
    clear_all(a, hist);
    return DS_SUCCESS;
}