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
/*======================================================================
 *  src/ds/next_next_practice.c
 *  雑多なデモ用 DS/Algo 実装
 *  2025‑07 ガイドライン（Opaque 型 + 抽象アロケータ DI）準拠
 *====================================================================*/

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
>>>>>>> feature

/* ◇ Undo‑Redo 用リングバッファ風システム                                */
typedef struct {
    ds_dlist_t undo;
    ds_dlist_t redo;
    ds_dlist_t data;
} ds_undo_redo_system_t;

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
struct ds_next_next_practice {
    const ds_allocator_t   *alloc; /* create 時に保存 → 破棄時に使用 */
    ds_undo_redo_system_t   sys;
};
>>>>>>> feature

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
    *n = (ds_node_t){
        .value = v,
        .next  = NULL,
        .prev  = NULL,
        .id    = ++g_nodes_created
    };
    *out = n;
>>>>>>> feature
    return DS_SUCCESS;
}

/* ノード破棄 */
static inline void node_destroy(const ds_allocator_t *a, ds_node_t *n)
{
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
    g_nodes_freed++;
>>>>>>> feature
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
<<<<<<< HEAD
>>>>>>> feature
=======
>>>>>>> feature
