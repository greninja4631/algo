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
#include "ds/next_next_practice.h"
#include "util/logger.h"

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

/* ─────────── 内部構造体 ─────────── */

/* 双方向ノード */
>>>>>>> feature
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
>>>>>>> feature

/* Undo-Redo システム */
typedef struct {
    ds_dlist_t undo;
    ds_dlist_t redo;
    ds_dlist_t data;
} ds_undo_redo_system_t;

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
struct ds_next_next_practice {
    ds_undo_redo_system_t sys;
};
>>>>>>> feature

/* ─────────── 内部メトリクス ─────────── */
static uint64_t g_nodes_created = 0;
static uint64_t g_nodes_freed   = 0;

/* ─────────── 内部ユーティリティ ─────────── */
static ds_error_t node_create(int32_t v, ds_node_t **out)
{
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

static inline void node_destroy(ds_node_t *n)
{
    if (!n) return;
    free(n);
<<<<<<< HEAD
    ds_next_total_nodes_freed++;
>>>>>>> feature
=======
    g_nodes_freed++;
>>>>>>> feature
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
>>>>>>> feature
