/*───────────────────────────────────────────────
  next_next_practice.c
  デモ用データ構造＆アルゴリズム集
  ※ API 統一基準 2025-07 準拠
───────────────────────────────────────────────*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds/next_next_practice.h"
#include "util/logger.h"       /* ds_log / ds_log_level_t */

/* ==========================================================
 * 1. 内部型定義（ds_***_t で統一／ヘッダへは forward のみ）
 * ==========================================================*/
typedef struct ds_node {
    int32_t         value;
    struct ds_node *next;
    struct ds_node *prev;
    uint64_t        id;
} ds_node_t;

typedef struct {
    ds_node_t *head;
    ds_node_t *tail;
    size_t     size;
    bool       is_circular;
} ds_dlist_t;

typedef struct {
    ds_dlist_t undo;
    ds_dlist_t redo;
    ds_dlist_t data;
} ds_undo_redo_system_t;

/* ==========================================================
 * 2. メトリクス（静的・外部公開しないのでプレフィクス付き）
 * ==========================================================*/
static uint64_t ds_next_total_nodes_created = 0;
static uint64_t ds_next_total_nodes_freed   = 0;

/* ==========================================================
 * 3. ノードユーティリティ
 * ==========================================================*/
static ds_error_t
ds_next_node_create(int32_t value, ds_node_t **out_node)
{
    if (!out_node) return DS_ERR_NULL_POINTER;

    ds_node_t *n = malloc(sizeof(*n));
    if (!n) {
        ds_log(DS_LOG_LEVEL_ERROR, "malloc failed in %s", __func__);
        return DS_ERR_ALLOC;
    }
    n->value = value;
    n->next  = n->prev = NULL;
    n->id    = ++ds_next_total_nodes_created;
    *out_node = n;
    return DS_SUCCESS;
}

static void
ds_next_node_destroy(ds_node_t *n)
{
    if (!n) return;
    memset(n, 0, sizeof(*n));
    free(n);
    ds_next_total_nodes_freed++;
}

/* ==========================================================
 * 4. 双方向リスト（スタック／キュー兼用）
 * ==========================================================*/
static ds_error_t
ds_next_dlist_init(ds_dlist_t *lst)
{
    if (!lst) return DS_ERR_NULL_POINTER;
    memset(lst, 0, sizeof(*lst));
    return DS_SUCCESS;
}

static ds_error_t
ds_next_stack_push(ds_dlist_t *lst, int32_t value)
{
    if (!lst) return DS_ERR_NULL_POINTER;
    ds_node_t *n; ds_error_t r = ds_next_node_create(value, &n);
    if (r != DS_SUCCESS) return r;

    if (!lst->head) lst->head = lst->tail = n;
    else {
        n->next          = lst->head;
        lst->head->prev  = n;
        lst->head        = n;
    }
    lst->size++;
    return DS_SUCCESS;
}

static ds_error_t
ds_next_stack_pop(ds_dlist_t *lst, int32_t *out_value)
{
    if (!lst || !out_value) return DS_ERR_NULL_POINTER;
    if (!lst->head)        return DS_ERR_EMPTY;

    ds_node_t *old = lst->head;
    *out_value = old->value;

    lst->head = old->next;
    if (lst->head) lst->head->prev = NULL;
    else           lst->tail = NULL;

    ds_next_node_destroy(old);
    lst->size--;
    return DS_SUCCESS;
}

/*───────────────────────────────────────────────
  ここが **未使用** として警告されていた関数。
  署名の前に attribute を付けて抑止します。
───────────────────────────────────────────────*/
static __attribute__((unused)) ds_error_t
ds_next_queue_enqueue(ds_dlist_t *lst, int32_t value)
{
    if (!lst) return DS_ERR_NULL_POINTER;
    ds_node_t *n; ds_error_t r = ds_next_node_create(value, &n);
    if (r != DS_SUCCESS) return r;

    if (!lst->tail) lst->head = lst->tail = n;
    else {
        lst->tail->next = n;
        n->prev         = lst->tail;
        lst->tail       = n;
    }
    lst->size++;
    return DS_SUCCESS;
}

static inline ds_error_t
ds_next_queue_dequeue(ds_dlist_t *lst, int32_t *out_value)
{ return ds_next_stack_pop(lst, out_value); }

/* ==========================================================
 * 5. リストの可視化（デモ用）
 * ==========================================================*/
static void
ds_next_dlist_print(const ds_dlist_t *lst, size_t max_n)
{
    if (!lst) { puts("(null list)"); return; }
    printf("[size=%zu] ", lst->size);
    const ds_node_t *cur = lst->head; size_t cnt = 0;
    while (cur && cnt < max_n) {
        printf("%d%s", cur->value,
               (++cnt < lst->size && cnt < max_n) ? " -> " : "");
        cur = cur->next;
    }
    puts("");
}

/* ==========================================================
 * 6. Undo / Redo システム
 * ==========================================================*/
static ds_error_t
ds_next_undo_redo_init(ds_undo_redo_system_t *sys)
{
    if (!sys) return DS_ERR_NULL_POINTER;
    ds_error_t r;
    if ((r = ds_next_dlist_init(&sys->undo)) != DS_SUCCESS) return r;
    if ((r = ds_next_dlist_init(&sys->redo)) != DS_SUCCESS) return r;
    if ((r = ds_next_dlist_init(&sys->data)) != DS_SUCCESS) return r;
    return DS_SUCCESS;
}

static ds_error_t
ds_next_execute(ds_undo_redo_system_t *sys, int32_t value)
{
    if (!sys) return DS_ERR_NULL_POINTER;
    ds_error_t r;
    if ((r = ds_next_stack_push(&sys->data, value)) != DS_SUCCESS) return r;
    if ((r = ds_next_stack_push(&sys->undo, value)) != DS_SUCCESS) return r;
    /* redo スタックはクリア */
    sys->redo.head = sys->redo.tail = NULL;
    sys->redo.size = 0;
    return DS_SUCCESS;
}

static ds_error_t
ds_next_undo(ds_undo_redo_system_t *sys)
{
    if (!sys) return DS_ERR_NULL_POINTER;
    int32_t v;
    ds_error_t r = ds_next_stack_pop(&sys->undo, &v);
    if (r != DS_SUCCESS) return r;
    (void)ds_next_stack_pop(&sys->data, &v);
    ds_next_stack_push(&sys->redo, v);
    return DS_SUCCESS;
}

static ds_error_t
ds_next_redo(ds_undo_redo_system_t *sys)
{
    if (!sys) return DS_ERR_NULL_POINTER;
    int32_t v;
    ds_error_t r = ds_next_stack_pop(&sys->redo, &v);
    if (r != DS_SUCCESS) return r;
    ds_next_stack_push(&sys->data, v);
    ds_next_stack_push(&sys->undo, v);
    return DS_SUCCESS;
}

/* ==========================================================
 * 7. デモ関数群（unused 属性で警告抑止）
 * ==========================================================*/
__attribute__((unused))
static ds_error_t
ds_next_demo_array_ops(void)
{
    puts("=== Array Ops Demo ===");
    int arr[5] = { 10, 20, 30, 40, 50 };
    printf("arr[2] = %d\n", arr[2]);
    puts("Fixed-size array vs dynamic list explained.");
    return DS_SUCCESS;
}

__attribute__((unused))
static ds_error_t
ds_next_demo_stack(void)
{
    puts("=== Stack Demo ===");
    ds_dlist_t st; ds_next_dlist_init(&st);
    ds_next_stack_push(&st, 1);
    ds_next_stack_push(&st, 2);
    ds_next_stack_push(&st, 3);
    ds_next_dlist_print(&st, 10);
    return DS_SUCCESS;
}

/* ==========================================================
 * 8. テスト用公開デモ関数
 * ==========================================================*/
ds_error_t
ds_next_run_all_demos(void)
{
    ds_undo_redo_system_t sys;
    ds_error_t r = ds_next_undo_redo_init(&sys);
    if (r != DS_SUCCESS) return r;

    ds_next_execute(&sys, 10);
    ds_next_execute(&sys, 20);
    ds_next_execute(&sys, 30);
    ds_next_undo(&sys);
    ds_next_redo(&sys);

    ds_next_dlist_print(&sys.data, 10);
    /* unused デモ呼び出しで -Wunused-function 抑止 */
    ds_next_demo_array_ops();
    ds_next_demo_stack();

    return DS_SUCCESS;
}