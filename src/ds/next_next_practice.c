<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#include "logger.h"
<<<<<<< HEAD
#include "../../include/data_structures.h"
#include "../../include/ds/next_next_practice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

// 本体
struct next_next_practice {
    int dummy; // 必要な内部フィールド（例：今後拡張）
};

//--------------------
// 型定義・内部構造
//--------------------
=======
#include "data_structures.h"/
=======
/*───────────────────────────────────────────────
  next_next_practice.c
  デモ用データ構造＆アルゴリズム集
  ※ API 統一基準 2025-07 準拠
───────────────────────────────────────────────*/
>>>>>>> feature
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
=======
/**
 * @file  src/ds/next_next_practice.c
 * @brief 雑多なデモ用 DS/Algo 実装
 * @note  2025-07 ガイドライン (Opaque 型) 準拠
 */
>>>>>>> feature
=======
=======
#include <stdint.h>
#include <stddef.h>
>>>>>>> feature
/*======================================================================
 *  src/ds/next_next_practice.c
 *  Demo 実装 – Undo/Redo & Stack/Queue
 *  2025-07 ガイドライン準拠
 *    • Opaque 型 + 抽象アロケータ DI
 *    • NULL-safe / Valgrind-clean / clang-tidy & cppcheck zero-warning
 *====================================================================*/

<<<<<<< HEAD
>>>>>>> feature
#include "ds/next_next_practice.h"
#include "util/memory.h"      /* ds_malloc / ds_free / allocator helpers */
#include "util/logger.h"      /* ds_log()                              */

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
//--------------------
// 型定義（APIは data_structures.h 移譲を推奨！）
typedef enum {
    DLL_SUCCESS = 0,
    DLL_ERROR_NULL_POINTER,
    DLL_ERROR_MEMORY_ALLOC,
    DLL_ERROR_EMPTY_LIST,
    DLL_ERROR_INVALID_PARAM,
    DLL_ERROR_CIRCULAR_DETECT
} DllErrorCode;

>>>>>>> feature
typedef struct Node {
    int32_t data;
    struct Node *next;
    struct Node *prev;
<<<<<<< HEAD
=======
    uint64_t node_id;
>>>>>>> feature
} Node;

typedef struct {
    Node *head;
    Node *tail;
    size_t size;
    bool is_circular;
<<<<<<< HEAD
=======
    uint64_t next_node_id;
>>>>>>> feature
} DoublyLinkedList;
=======
/* ==========================================================
 * 1. 内部型定義（ds_***_t で統一／ヘッダへは forward のみ）
 * ==========================================================*/
=======
#include <stdlib.h>   /* malloc / free */
#include <string.h>   /* memset */
=======
#include <string.h>            /* memset */
#include <inttypes.h>          /* int32_t,uint64_t                     */
>>>>>>> feature

/* ────────────────────────────────────────────────────────────────
 * 内部構造体定義（外部ヘッダには一切公開しない）
 *────────────────────────────────────────────────────────────── */

<<<<<<< HEAD
/* 双方向ノード */
>>>>>>> feature
=======
/* ◇ 双方向ノード                                                        */
>>>>>>> feature
=======
#include "ds/next_next_practice.h" /* 公開ヘッダ */
#include "util/memory.h"           /* ds_malloc / ds_free */
#include "util/logger.h"           /* ds_log             */
#include <string.h>
#include <inttypes.h>

/*──────────────────────────────────────
 * 1. 内部データ構造
 *─────────────────────────────────────*/

/* ノード */
>>>>>>> feature
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
>>>>>>> feature

/* Undo/Redo システム */
typedef struct {
    ds_dlist_t undo;
    ds_dlist_t redo;
    ds_dlist_t data;
} ds_undo_redo_system_t;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
//--------------------
<<<<<<< HEAD
// API: 本体作成・破棄
//--------------------
next_next_practice_t* next_next_practice_create(void) {
    next_next_practice_t* obj = (next_next_practice_t*)ds_malloc(sizeof(next_next_practice_t));
    if (!obj) {
        ds_log(DS_LOG_ERROR, "[next_next_practice] create失敗\n");
        return NULL;
    }
    obj->dummy = 0;
    return obj;
}

void next_next_practice_destroy(next_next_practice_t* obj) {
    if (obj) ds_free(obj);
}

//--------------------
// ヘルパー関数（省略可: 既存の内容そのまま）
//--------------------
static Node* create_node(int32_t data) {
    Node* node = (Node*)ds_malloc(sizeof(Node));
    if (!node) {
        ds_log(DS_LOG_ERROR, "[next_next_practice] メモリ確保失敗 (create_node)\n");
        return NULL;
    }
    node->data = data;
    node->next = node->prev = NULL;
    return node;
}

static void free_node(Node* node) {
    if (node) ds_free(node);
=======
// グローバルメトリクス（テスト用）
static uint64_t g_total_nodes_created = 0;
static uint64_t g_total_nodes_freed = 0;
=======
/* ==========================================================
 * 2. メトリクス（静的・外部公開しないのでプレフィクス付き）
 * ==========================================================*/
static uint64_t ds_next_total_nodes_created = 0;
static uint64_t ds_next_total_nodes_freed   = 0;
>>>>>>> feature
=======
/* Opaque ハンドル本体（外部には見えない） */
=======
/* ◇ Opaque 本体                                                            */
>>>>>>> feature
=======
/* Opaque 本体 */
>>>>>>> feature
struct ds_next_next_practice {
    const ds_allocator_t   *alloc; /* DI アロケータ */
    ds_undo_redo_system_t    sys;  /* 内部状態      */
};
>>>>>>> feature

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
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    if (!out_node) return DS_ERR_NULL_POINTER;

    ds_node_t *n = malloc(sizeof(*n));
    if (!n) {
        ds_log(DS_LOG_LEVEL_ERROR, "malloc failed in %s", __func__);
        return DS_ERR_ALLOC;
    }
<<<<<<< HEAD
    node->data = data; node->next = node->prev = NULL;
    node->node_id = ++g_total_nodes_created;
    *error_code = DLL_SUCCESS;
    return node;
}
static void free_node_safe(Node *node) {
    if (!node) return;
    memset(node, 0, sizeof(Node));
    free(node); g_total_nodes_freed++;
>>>>>>> feature
}

//--------------------
// リスト操作
<<<<<<< HEAD
//--------------------
static ds_error_t init_list(DoublyLinkedList* list) { /* ... */ }
static ds_error_t stack_push(DoublyLinkedList* list, int32_t data) { /* ... */ }
static ds_error_t stack_pop(DoublyLinkedList* list, int32_t* data) { /* ... */ }
static ds_error_t queue_enqueue(DoublyLinkedList* list, int32_t data) { /* ... */ }
static ds_error_t queue_dequeue(DoublyLinkedList* list, int32_t* data) { /* ... */ }
static ds_error_t make_circular(DoublyLinkedList* list) { /* ... */ }
static void print_list(const DoublyLinkedList* list, size_t max_display) { /* ... */ }
static ds_error_t free_list(DoublyLinkedList* list) { /* ... */ }

//--------------------
// Undo/Redoサンプル
//--------------------
static ds_error_t init_undo_redo_system(UndoRedoSystem* sys) { /* ... */ }
static ds_error_t execute_operation(UndoRedoSystem* sys, int32_t data) { /* ... */ }
static ds_error_t perform_undo(UndoRedoSystem* sys) { /* ... */ }
static ds_error_t perform_redo(UndoRedoSystem* sys) { /* ... */ }

//--------------------
// ラウンドロビン
//--------------------
static ds_error_t simulate_round_robin(DoublyLinkedList* list, uint32_t time_slice, uint32_t max_cycles) { /* ... */ }

//--------------------
// テスト関数
//--------------------
ds_error_t demonstrate_array_operations(void) { /* ... */ }
ds_error_t test_stack_operations(void) { /* ... */ }
ds_error_t test_queue_operations(void) { /* ... */ }
ds_error_t test_undo_redo_system(void) { /* ... */ }
ds_error_t test_circular_and_round_robin(void) { /* ... */ }

// ※各static関数は既存コード通り。省略部分はユーザー提示の実装内容を貼り付ければOK。

=======
static DllErrorCode init_list(DoublyLinkedList *list) {
    if (!list) return DLL_ERROR_NULL_POINTER;
    list->head = list->tail = NULL; list->size = 0; list->is_circular = false; list->next_node_id = 1;
    return DLL_SUCCESS;
=======
    n->value = value;
    n->next  = n->prev = NULL;
    n->id    = ++ds_next_total_nodes_created;
    *out_node = n;
=======
    if (!out) return DS_ERR_NULL_POINTER;
    ds_node_t *n = malloc(sizeof *n);
    if (!n)   return DS_ERR_ALLOC;
=======
    if (!a || !out) return DS_ERR_NULL_POINTER;
    ds_node_t *n = ds_malloc(a, 1, sizeof *n);
    if (!n) return DS_ERR_ALLOC;
>>>>>>> feature
=======
    if (!alloc)      return DS_ERR_NULL_POINTER; /* guard-1  */
    if (!out_node)   return DS_ERR_NULL_POINTER; /* guard-2  */

    ds_node_t *n = ds_malloc(alloc, 1, sizeof *n);
    if (!n) return DS_ERR_ALLOC;

>>>>>>> feature
    *n = (ds_node_t){
        .value = value,
        .next  = NULL,
        .prev  = NULL,
        .id    = ++g_nodes_created
    };
<<<<<<< HEAD
    *out = n;
>>>>>>> feature
=======
    *out_node = n;
>>>>>>> feature
    return DS_SUCCESS;
}

static inline void
node_destroy(const ds_allocator_t *alloc, ds_node_t *n)
{
<<<<<<< HEAD
<<<<<<< HEAD
    if (!n) return;
    free(n);
<<<<<<< HEAD
    ds_next_total_nodes_freed++;
>>>>>>> feature
=======
=======
    if (!a || !n) return;
    ds_free(a, n);
>>>>>>> feature
=======
    if (!alloc || !n) return;
    ds_free(alloc, n);
>>>>>>> feature
    g_nodes_freed++;
>>>>>>> feature
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

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
// ↓（mainより前）に必要な関数・グローバル変数・型は必ず宣言・定義しておくこと！
>>>>>>> feature
=======
static ds_error_t
ds_next_undo(ds_undo_redo_system_t *sys)
=======
static ds_error_t ur_undo(ds_undo_redo_system_t *s)
>>>>>>> feature
=======
static ds_error_t ur_undo(const ds_allocator_t *a, ds_undo_redo_system_t *s)
>>>>>>> feature
=======
static ds_error_t
ur_undo(const ds_allocator_t *alloc, ds_undo_redo_system_t *sys)
>>>>>>> feature
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
<<<<<<< HEAD
}
<<<<<<< HEAD
>>>>>>> feature
=======
>>>>>>> feature
=======
}
>>>>>>> feature
