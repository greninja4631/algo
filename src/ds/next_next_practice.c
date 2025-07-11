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
#include "ds/next_next_practice.h"
#include "util/logger.h"       /* ds_log / ds_log_level_t */

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
>>>>>>> feature

typedef struct {
    ds_dlist_t undo;
    ds_dlist_t redo;
    ds_dlist_t data;
} ds_undo_redo_system_t;

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
    return DS_SUCCESS;
}

static void
ds_next_node_destroy(ds_node_t *n)
{
    if (!n) return;
    memset(n, 0, sizeof(*n));
    free(n);
    ds_next_total_nodes_freed++;
>>>>>>> feature
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

<<<<<<< HEAD
// ↓（mainより前）に必要な関数・グローバル変数・型は必ず宣言・定義しておくこと！
>>>>>>> feature
=======
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
>>>>>>> feature
