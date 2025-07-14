/**
 * @file  src/ds/next_next_practice.c
 * @brief 雑多なデモ用 DS/Algo 実装
 * @note  2025-07 ガイドライン (Opaque 型) 準拠
 */
#include "ds/next_next_practice.h"
#include "util/logger.h"

#include <stdlib.h>   /* malloc / free */
#include <string.h>   /* memset */

/* ─────────── 内部構造体 ─────────── */

/* 双方向ノード */
typedef struct ds_node {
    int32_t               value;
    struct ds_node       *next;
    struct ds_node       *prev;
    uint64_t              id;
} ds_node_t;

/* 双方向リスト（Stack/Queue 共通） */
typedef struct {
    ds_node_t *head;
    ds_node_t *tail;
    size_t     size;
} ds_dlist_t;

/* Undo-Redo システム */
typedef struct {
    ds_dlist_t undo;
    ds_dlist_t redo;
    ds_dlist_t data;
} ds_undo_redo_system_t;

/* Opaque ハンドル本体（外部には見えない） */
struct ds_next_next_practice {
    ds_undo_redo_system_t sys;
};

/* ─────────── 内部メトリクス ─────────── */
static uint64_t g_nodes_created = 0;
static uint64_t g_nodes_freed   = 0;

/* ─────────── 内部ユーティリティ ─────────── */
static ds_error_t node_create(int32_t v, ds_node_t **out)
{
    if (!out) return DS_ERR_NULL_POINTER;
    ds_node_t *n = malloc(sizeof *n);
    if (!n)   return DS_ERR_ALLOC;
    *n = (ds_node_t){
        .value = v,
        .next  = NULL,
        .prev  = NULL,
        .id    = ++g_nodes_created
    };
    *out = n;
    return DS_SUCCESS;
}

static inline void node_destroy(ds_node_t *n)
{
    if (!n) return;
    free(n);
    g_nodes_freed++;
}

/* dlist 基本 */
static inline void dlist_init(ds_dlist_t *lst)
{
    memset(lst, 0, sizeof *lst);
}

/* dlist の全ノードを破棄 */
static void dlist_clear(ds_dlist_t *lst)
{
    if (!lst) return;
    ds_node_t *cur = lst->head;
    while (cur) {
        ds_node_t *next = cur->next;
        node_destroy(cur);
        cur = next;
    }
    lst->head = lst->tail = NULL;
    lst->size = 0;
}

/* Stack push/pop */
static ds_error_t stack_push(ds_dlist_t *lst, int32_t v)
{
    if (!lst) return DS_ERR_NULL_POINTER;
    ds_node_t *n;
    ds_error_t r = node_create(v, &n);
    if (r) return r;
    if (!lst->head) {
        lst->head = lst->tail = n;
    } else {
        n->next = lst->head;
        lst->head->prev = n;
        lst->head = n;
    }
    lst->size++;
    return DS_SUCCESS;
}

static ds_error_t stack_pop(ds_dlist_t *lst, int32_t *out_v)
{
    if (!lst || !out_v) return DS_ERR_NULL_POINTER;
    if (!lst->head)    return DS_ERR_EMPTY;
    ds_node_t *old = lst->head;
    *out_v = old->value;
    lst->head = old->next;
    if (lst->head) {
        lst->head->prev = NULL;
    } else {
        lst->tail = NULL;
    }
    node_destroy(old);
    lst->size--;
    return DS_SUCCESS;
}

/* Queue enqueue/dequeue（enqueue 未使用でも attribute で警告抑止） */
static __attribute__((unused)) ds_error_t queue_enqueue(ds_dlist_t *lst, int32_t v)
{
    if (!lst) return DS_ERR_NULL_POINTER;
    ds_node_t *n;
    ds_error_t r = node_create(v, &n);
    if (r) return r;
    if (!lst->tail) {
        lst->head = lst->tail = n;
    } else {
        lst->tail->next = n;
        n->prev = lst->tail;
        lst->tail = n;
    }
    lst->size++;
    return DS_SUCCESS;
}

static inline ds_error_t queue_dequeue(ds_dlist_t *lst, int32_t *out_v)
{
    return stack_pop(lst, out_v);
}

/* Undo-Redo */
static ds_error_t ur_init(ds_undo_redo_system_t *s)
{
    if (!s) return DS_ERR_NULL_POINTER;
    dlist_init(&s->undo);
    dlist_init(&s->redo);
    dlist_init(&s->data);
    return DS_SUCCESS;
}

static ds_error_t ur_execute(ds_undo_redo_system_t *s, int32_t v)
{
    if (!s) return DS_ERR_NULL_POINTER;
    stack_push(&s->data, v);
    stack_push(&s->undo, v);
    dlist_clear(&s->redo);  /* redo クリア */
    return DS_SUCCESS;
}

static ds_error_t ur_undo(ds_undo_redo_system_t *s)
{
    if (!s) return DS_ERR_NULL_POINTER;
    int32_t v;
    if (stack_pop(&s->undo, &v)) return DS_ERR_EMPTY;
    stack_pop(&s->data, &v);
    stack_push(&s->redo, v);
    return DS_SUCCESS;
}

static ds_error_t ur_redo(ds_undo_redo_system_t *s)
{
    if (!s) return DS_ERR_NULL_POINTER;
    int32_t v;
    if (stack_pop(&s->redo, &v)) return DS_ERR_EMPTY;
    stack_push(&s->data, v);
    stack_push(&s->undo, v);
    return DS_SUCCESS;
}

/* ─────────── Opaque API ─────────── */
ds_error_t ds_next_next_practice_create(ds_next_next_practice_t **out_p)
{
    if (!out_p) return DS_ERR_NULL_POINTER;
    ds_next_next_practice_t *p = malloc(sizeof *p);
    if (!p) return DS_ERR_ALLOC;
    ur_init(&p->sys);
    *out_p = p;
    return DS_SUCCESS;
}

void ds_next_next_practice_destroy(ds_next_next_practice_t *p)
{
    if (!p) return;
    /* 作成した全ノードを解放 */
    dlist_clear(&p->sys.data);
    dlist_clear(&p->sys.undo);
    dlist_clear(&p->sys.redo);
    free(p);
}

/* デモ一括実行：テストコードが呼び出す唯一の公開関数 */
ds_error_t ds_next_next_practice_run_all_demos(ds_next_next_practice_t *p)
{
    if (!p) return DS_ERR_NULL_POINTER;
    ds_undo_redo_system_t *s = &p->sys;
    ur_execute(s, 10);
    ur_execute(s, 20);
    ur_execute(s, 30);
    ur_undo(s);
    ur_redo(s);
    ds_log(DS_LOG_LEVEL_INFO, "Demo finished. data size=%zu", s->data.size);
    return DS_SUCCESS;
}