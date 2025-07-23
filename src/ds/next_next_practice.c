#include <stdint.h>
#include <stddef.h>
/*======================================================================
 *  src/ds/next_next_practice.c
 *  Demo 実装 – Undo/Redo & Stack/Queue
 *  2025-07 ガイドライン準拠
 *    • Opaque 型 + 抽象アロケータ DI
 *    • NULL-safe / Valgrind-clean / clang-tidy & cppcheck zero-warning
 *====================================================================*/

#include "ds/next_next_practice.h" /* 公開ヘッダ */
#include "util/memory.h"           /* ds_malloc / ds_free */
#include "util/logger.h"           /* ds_log             */
#include <string.h>
#include <inttypes.h>

/*──────────────────────────────────────
 * 1. 内部データ構造
 *─────────────────────────────────────*/

/* ノード */
typedef struct ds_node {
    int32_t          value;
    struct ds_node  *next;
    struct ds_node  *prev;
    uint64_t         id;
} ds_node_t;

/* 汎用双方向リスト（Stack と Queue を共用） */
typedef struct {
    ds_node_t *head;
    ds_node_t *tail;
    size_t     size;
} ds_dlist_t;

/* Undo/Redo システム */
typedef struct {
    ds_dlist_t undo;
    ds_dlist_t redo;
    ds_dlist_t data;
} ds_undo_redo_system_t;

/* Opaque 本体 */
struct ds_next_next_practice {
    const ds_allocator_t   *alloc; /* DI アロケータ */
    ds_undo_redo_system_t    sys;  /* 内部状態      */
};

/*──────────────────────────────────────
 * 2. デバッグ統計
 *─────────────────────────────────────*/
static uint64_t g_nodes_created = 0;
static uint64_t g_nodes_freed   = 0;

/*──────────────────────────────────────
 * 3. 内部ユーティリティ
 *─────────────────────────────────────*/

/* --- ノード生成 / 破棄 ------------------------------------------------- */
static ds_error_t
node_create(const ds_allocator_t *alloc, int32_t value, ds_node_t **out_node)
{
    if (!alloc)      return DS_ERR_NULL_POINTER; /* guard-1  */
    if (!out_node)   return DS_ERR_NULL_POINTER; /* guard-2  */

    ds_node_t *n = ds_malloc(alloc, 1, sizeof *n);
    if (!n) return DS_ERR_ALLOC;

    *n = (ds_node_t){
        .value = value,
        .next  = NULL,
        .prev  = NULL,
        .id    = ++g_nodes_created
    };
    *out_node = n;
    return DS_SUCCESS;
}

static inline void
node_destroy(const ds_allocator_t *alloc, ds_node_t *n)
{
    if (!alloc || !n) return;
    ds_free(alloc, n);
    g_nodes_freed++;
}

/* --- dlist 初期化 / 全解放 ------------------------------------------- */
static inline void dlist_init(ds_dlist_t *list)
{
    if (list) memset(list, 0, sizeof *list);
}

static void
dlist_clear(const ds_allocator_t *alloc, ds_dlist_t *list)
{
    if (!alloc) return;            /* cppcheck guard               */
    if (!list)  return;            /* NOLINT(readability-null-check) */

    for (ds_node_t *cur = list->head; cur; ) {
        ds_node_t *next = cur->next;
        node_destroy(alloc, cur);
        cur = next;
    }
    list->head = list->tail = NULL;
    list->size = 0;
}

/* --- Stack push / pop ------------------------------------------------- */
static ds_error_t
stack_push(const ds_allocator_t *alloc, ds_dlist_t *list, int32_t v)
{
    if (!alloc) return DS_ERR_NULL_POINTER;
    if (!list)  return DS_ERR_NULL_POINTER;      /* guard-2 */

    ds_node_t *n = NULL;
    ds_error_t rc = node_create(alloc, v, &n);
    if (rc) return rc;

    if (!list->head) {
        list->head = list->tail = n;
    } else {
        n->next = list->head;
        list->head->prev = n;
        list->head = n;
    }
    list->size++;
    return DS_SUCCESS;
}

static ds_error_t
stack_pop(const ds_allocator_t *alloc, ds_dlist_t *list, int32_t *out_v)
{
    if (!alloc)   return DS_ERR_NULL_POINTER;
    if (!list ||
        !out_v)   return DS_ERR_NULL_POINTER;

    if (!list->head) return DS_ERR_EMPTY;

    ds_node_t *old = list->head;
    *out_v = old->value;

    list->head = old->next;
    if (list->head) list->head->prev = NULL;
    else            list->tail = NULL;

    node_destroy(alloc, old);
    list->size--;
    return DS_SUCCESS;
}

/* --- Queue enqueue / dequeue ----------------------------------------- */
static __attribute__((unused)) ds_error_t
queue_enqueue(const ds_allocator_t *alloc, ds_dlist_t *list, int32_t v)
{
    if (!alloc) return DS_ERR_NULL_POINTER;
    if (!list)  return DS_ERR_NULL_POINTER;

    ds_node_t *n = NULL;
    ds_error_t rc = node_create(alloc, v, &n);
    if (rc) return rc;

    if (!list->tail) {
        list->head = list->tail = n;
    } else {
        list->tail->next = n;
        n->prev = list->tail;
        list->tail = n;
    }
    list->size++;
    return DS_SUCCESS;
}

static inline ds_error_t
queue_dequeue(const ds_allocator_t *alloc, ds_dlist_t *list, int32_t *out_v)
{
    return stack_pop(alloc, list, out_v); /* dequeue = pop head */
}

/* --- Undo / Redo ------------------------------------------------------ */
static ds_error_t ur_init(ds_undo_redo_system_t *sys)
{
    if (!sys) return DS_ERR_NULL_POINTER;
    dlist_init(&sys->undo);
    dlist_init(&sys->redo);
    dlist_init(&sys->data);
    return DS_SUCCESS;
}

static ds_error_t
ur_execute(const ds_allocator_t *alloc, ds_undo_redo_system_t *sys, int32_t v)
{
    if (!alloc || !sys) return DS_ERR_NULL_POINTER;
    stack_push(alloc, &sys->data, v);
    stack_push(alloc, &sys->undo, v);
    dlist_clear(alloc, &sys->redo); /* redo 履歴を破棄 */
    return DS_SUCCESS;
}

static ds_error_t
ur_undo(const ds_allocator_t *alloc, ds_undo_redo_system_t *sys)
{
    if (!alloc || !sys) return DS_ERR_NULL_POINTER;
    int32_t v;
    if (stack_pop(alloc, &sys->undo, &v)) return DS_ERR_EMPTY;
    stack_pop(alloc, &sys->data, &v);
    stack_push(alloc, &sys->redo, v);
    return DS_SUCCESS;
}

static ds_error_t
ur_redo(const ds_allocator_t *alloc, ds_undo_redo_system_t *sys)
{
    if (!alloc || !sys) return DS_ERR_NULL_POINTER;
    int32_t v;
    if (stack_pop(alloc, &sys->redo, &v)) return DS_ERR_EMPTY;
    stack_push(alloc, &sys->data, v);
    stack_push(alloc, &sys->undo, v);
    return DS_SUCCESS;
}

/*──────────────────────────────────────
 * 4. 公開 API
 *─────────────────────────────────────*/

ds_error_t
ds_next_next_practice_create(const ds_allocator_t *alloc,
                             ds_next_next_practice_t **out_obj)
{
    if (!alloc || !out_obj) return DS_ERR_NULL_POINTER;

    ds_next_next_practice_t *o = ds_malloc(alloc, 1, sizeof *o);
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
    if (!obj)   return DS_SUCCESS; /* NULL-safe */

    dlist_clear(alloc, &obj->sys.data);
    dlist_clear(alloc, &obj->sys.undo);
    dlist_clear(alloc, &obj->sys.redo);

    ds_free(alloc, obj);
    return DS_SUCCESS;
}

ds_error_t
ds_next_next_practice_run_all_demos(const ds_allocator_t *alloc,
                                    ds_next_next_practice_t *obj)
{
    if (!alloc || !obj)             // NOLINT(clang-analyzer-core.NullDereference)
        return DS_ERR_NULL_POINTER;

    ds_undo_redo_system_t *sys = &obj->sys;

    ur_execute(alloc, sys, 10);
    ur_execute(alloc, sys, 20);
    ur_execute(alloc, sys, 30);
    ur_undo   (alloc, sys);
    ur_redo   (alloc, sys);

    ds_log(DS_LOG_LEVEL_INFO,
           "[next_next_practice] size=%zu, created=%" PRIu64 ", freed=%" PRIu64,
           sys->data.size, g_nodes_created, g_nodes_freed);
    return DS_SUCCESS;
}
