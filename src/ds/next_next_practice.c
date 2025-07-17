/*======================================================================
 *  src/ds/next_next_practice.c
 *  雑多なデモ用 DS/Algo 実装
 *  2025‑07 ガイドライン（Opaque 型 + 抽象アロケータ DI）準拠
 *====================================================================*/

#include "ds/next_next_practice.h"
#include "util/memory.h"      /* ds_malloc / ds_free / allocator helpers */
#include "util/logger.h"      /* ds_log()                              */

#include <string.h>            /* memset */
#include <inttypes.h>          /* int32_t,uint64_t                     */

/* ────────────────────────────────────────────────────────────────
 * 内部構造体定義（外部ヘッダには一切公開しない）
 *────────────────────────────────────────────────────────────── */

/* ◇ 双方向ノード                                                        */
typedef struct ds_node {
    int32_t             value;
    struct ds_node     *next;
    struct ds_node     *prev;
    uint64_t            id;      /* デバッグ用シリアル */
} ds_node_t;

/* ◇ 汎用双方向リスト（Stack / Queue 共有実装）                          */
typedef struct {
    ds_node_t *head;   /* 先頭(スタック Top / キュー Front) */
    ds_node_t *tail;   /* 末尾(スタック Bottom / キュー Rear)*/
    size_t     size;
} ds_dlist_t;

/* ◇ Undo‑Redo 用リングバッファ風システム                                */
typedef struct {
    ds_dlist_t undo;
    ds_dlist_t redo;
    ds_dlist_t data;
} ds_undo_redo_system_t;

/* ◇ Opaque 本体                                                            */
struct ds_next_next_practice {
    const ds_allocator_t   *alloc; /* create 時に保存 → 破棄時に使用 */
    ds_undo_redo_system_t   sys;
};

/* ────────────────────────────────────────────────────────────────
 *  内部メトリクス（デバッグ用）
 *────────────────────────────────────────────────────────────── */
static uint64_t g_nodes_created = 0;
static uint64_t g_nodes_freed   = 0;

/* ────────────────────────────────────────────────────────────────
 *  内部ユーティリティ
 *────────────────────────────────────────────────────────────── */

/* ノード生成 */
static ds_error_t node_create(const ds_allocator_t *a, int32_t v, ds_node_t **out)
{
    if (!a || !out) return DS_ERR_NULL_POINTER;
    ds_node_t *n = ds_malloc(a, 1, sizeof *n);
    if (!n) return DS_ERR_ALLOC;
    *n = (ds_node_t){
        .value = v,
        .next  = NULL,
        .prev  = NULL,
        .id    = ++g_nodes_created
    };
    *out = n;
    return DS_SUCCESS;
}

/* ノード破棄 */
static inline void node_destroy(const ds_allocator_t *a, ds_node_t *n)
{
    if (!a || !n) return;
    ds_free(a, n);
    g_nodes_freed++;
}

/* dlist 初期化 */
static inline void dlist_init(ds_dlist_t *lst)
{
    memset(lst, 0, sizeof *lst);
}

/* dlist 全ノード破棄 */
static void dlist_clear(const ds_allocator_t *a, ds_dlist_t *lst)
{
    if (!a || !lst) return;
    ds_node_t *cur = lst->head;
    while (cur) {
        ds_node_t *next = cur->next;
        node_destroy(a, cur);
        cur = next;
    }
    lst->head = lst->tail = NULL;
    lst->size = 0;
}

/* ==== Stack push/pop ==== */
static ds_error_t stack_push(const ds_allocator_t *a, ds_dlist_t *lst, int32_t v)
{
    if (!a || !lst) return DS_ERR_NULL_POINTER;
    ds_node_t *n;
    ds_error_t rc = node_create(a, v, &n);
    if (rc) return rc;

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

static ds_error_t stack_pop(const ds_allocator_t *a, ds_dlist_t *lst, int32_t *out_v)
{
    if (!a || !lst || !out_v) return DS_ERR_NULL_POINTER;
    if (!lst->head)            return DS_ERR_EMPTY;

    ds_node_t *old = lst->head;
    *out_v = old->value;

    lst->head = old->next;
    if (lst->head) lst->head->prev = NULL;
    else           lst->tail = NULL;

    node_destroy(a, old);
    lst->size--;
    return DS_SUCCESS;
}

/* ==== Queue enqueue/dequeue (enqueue は未使用だが実装保持) ==== */
static __attribute__((unused)) ds_error_t queue_enqueue(const ds_allocator_t *a, ds_dlist_t *lst, int32_t v)
{
    if (!a || !lst) return DS_ERR_NULL_POINTER;
    ds_node_t *n;
    ds_error_t rc = node_create(a, v, &n);
    if (rc) return rc;

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

static inline ds_error_t queue_dequeue(const ds_allocator_t *a, ds_dlist_t *lst, int32_t *out_v)
{
    return stack_pop(a, lst, out_v);   /* dequeue = pop head */
}

/* ==== Undo‑Redo ==== */
static ds_error_t ur_init(ds_undo_redo_system_t *s)
{
    if (!s) return DS_ERR_NULL_POINTER;
    dlist_init(&s->undo);
    dlist_init(&s->redo);
    dlist_init(&s->data);
    return DS_SUCCESS;
}

static ds_error_t ur_execute(const ds_allocator_t *a, ds_undo_redo_system_t *s, int32_t v)
{
    if (!a || !s) return DS_ERR_NULL_POINTER;
    stack_push(a, &s->data, v);
    stack_push(a, &s->undo, v);
    dlist_clear(a, &s->redo);           /* redo 履歴は消去 */
    return DS_SUCCESS;
}

static ds_error_t ur_undo(const ds_allocator_t *a, ds_undo_redo_system_t *s)
{
    if (!a || !s) return DS_ERR_NULL_POINTER;
    int32_t v;
    if (stack_pop(a, &s->undo, &v)) return DS_ERR_EMPTY;
    stack_pop(a, &s->data, &v);
    stack_push(a, &s->redo, v);
    return DS_SUCCESS;
}

static ds_error_t ur_redo(const ds_allocator_t *a, ds_undo_redo_system_t *s)
{
    if (!a || !s) return DS_ERR_NULL_POINTER;
    int32_t v;
    if (stack_pop(a, &s->redo, &v)) return DS_ERR_EMPTY;
    stack_push(a, &s->data, v);
    stack_push(a, &s->undo, v);
    return DS_SUCCESS;
}

/* ────────────────────────────────────────────────────────────────
 *  公開 API 実装
 *────────────────────────────────────────────────────────────── */

ds_error_t
ds_next_next_practice_create(const ds_allocator_t *alloc,
                             ds_next_next_practice_t **out_obj)
{
    if (!alloc || !out_obj) return DS_ERR_NULL_POINTER;

    ds_next_next_practice_t *o =
        ds_malloc(alloc, 1, sizeof *o);
    if (!o) return DS_ERR_ALLOC;

    o->alloc = alloc;
    ur_init(&o->sys);
    *out_obj = o;
    return DS_SUCCESS;
}


ds_error_t
ds_next_next_practice_destroy(const ds_allocator_t *alloc,
                              ds_next_next_practice_t *obj)
{
    if (!alloc) return DS_ERR_NULL_POINTER;
    if (!obj)   return DS_SUCCESS;        /* NULL セーフ */

    /* ノードをすべて解放 */
    dlist_clear(alloc, &obj->sys.data);
    dlist_clear(alloc, &obj->sys.undo);
    dlist_clear(alloc, &obj->sys.redo);

    ds_free(alloc, obj);
    return DS_SUCCESS;
}


ds_error_t
ds_next_next_practice_run_all_demos(ds_next_next_practice_t *obj)
{
    if (!obj) return DS_ERR_NULL_POINTER;
    const ds_allocator_t *a = obj->alloc;
    ds_undo_redo_system_t *s = &obj->sys;

    /* シンプルなデモシーケンス */
    ur_execute(a, s, 10);
    ur_execute(a, s, 20);
    ur_execute(a, s, 30);
    ur_undo   (a, s);
    ur_redo   (a, s);

    ds_log(DS_LOG_LEVEL_INFO,
           "[next_next_practice] demo finished — data.size=%zu, created=%" PRIu64 ", freed=%" PRIu64,
           s->data.size, g_nodes_created, g_nodes_freed);

    return DS_SUCCESS;
}
