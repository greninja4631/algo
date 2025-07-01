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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>  // getpid()

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

typedef struct {
    DoublyLinkedList undo_stack;
    DoublyLinkedList redo_stack;
    DoublyLinkedList main_data;
} UndoRedoSystem;

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

//--------------------
// ログ/観測/デバッグ

static void log_error(DllErrorCode error_code, const char *function_name, const char *info) {
    const char *error_messages[] = {
        "SUCCESS", "NULL_POINTER", "MEMORY_ALLOC", "EMPTY_LIST", "INVALID_PARAM", "CIRCULAR_DETECT"
    };
    fprintf(stderr, "[ERROR] %s in %s: %s\n", error_messages[error_code], function_name, info ? info : "");
}

// --- DEBUGフラグによる分岐で未使用警告ゼロ ---
// 「一度は呼ぶ」「空関数」どちらでもエラーゼロに
#ifdef DEBUG
static void log_debug(const char *format, ...) {
    va_list args; va_start(args, format);
    printf("[DEBUG] "); vprintf(format, args); printf("\n");
    va_end(args);
}
#else
static inline void log_debug(const char *format, ...) { (void)format; }
#endif

static void log_memory_stats(void) {
    printf("[MEMORY] Created: %lu, Freed: %lu, Leaked: %lu\n",
           g_total_nodes_created, g_total_nodes_freed,
           g_total_nodes_created - g_total_nodes_freed);
}

//--------------------
// コア（ノード・メモリ管理）
static Node* create_node_safe(int32_t data, DllErrorCode *error_code) {
    assert(error_code != NULL);
    Node *node = malloc(sizeof(Node));
    if (!node) {
        *error_code = DLL_ERROR_MEMORY_ALLOC;
        log_error(DLL_ERROR_MEMORY_ALLOC, __func__, "malloc failed");
        return NULL;
    }
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
}
static DllErrorCode stack_push(DoublyLinkedList *list, int32_t data) {
    if (!list) return DLL_ERROR_NULL_POINTER;
    DllErrorCode err; Node *n = create_node_safe(data, &err); if (!n) return err;
    if (!list->head) list->head = list->tail = n;
    else { n->next = list->head; list->head->prev = n; list->head = n; }
    list->size++;
    return DLL_SUCCESS;
}
static DllErrorCode stack_pop(DoublyLinkedList *list, int32_t *data) {
    if (!list || !data) return DLL_ERROR_NULL_POINTER;
    if (!list->head) return DLL_ERROR_EMPTY_LIST;
    Node *old_head = list->head; *data = old_head->data;
    list->head = old_head->next;
    if (list->head) list->head->prev = NULL;
    else list->tail = NULL;
    free_node_safe(old_head); list->size--;
    return DLL_SUCCESS;
}
static DllErrorCode queue_enqueue(DoublyLinkedList *list, int32_t data) {
    if (!list) return DLL_ERROR_NULL_POINTER;
    DllErrorCode err; Node *n = create_node_safe(data, &err); if (!n) return err;
    if (!list->tail) list->head = list->tail = n;
    else { list->tail->next = n; n->prev = list->tail; list->tail = n; }
    list->size++;
    return DLL_SUCCESS;
}
static DllErrorCode queue_dequeue(DoublyLinkedList *list, int32_t *data) {
    return stack_pop(list, data);
}
static DllErrorCode make_circular(DoublyLinkedList *list) {
    if (!list) return DLL_ERROR_NULL_POINTER;
    if (!list->head || !list->tail) return DLL_ERROR_EMPTY_LIST;
    if (list->is_circular) return DLL_SUCCESS;
    list->tail->next = list->head; list->head->prev = list->tail;
    list->is_circular = true;
    return DLL_SUCCESS;
}
static void print_list_safe(const DoublyLinkedList *list, size_t max_display) {
    if (!list) { printf("(NULL list)\n"); return; }
    if (!list->head) { printf("(empty list, size=%zu)\n", list->size); return; }
    printf("List (size=%zu, circular=%s): ", list->size, list->is_circular ? "yes" : "no");
    Node *cur = list->head; size_t cnt = 0;
    do {
        printf("%d", cur->data);
        if (cnt + 1 < max_display && cur->next && (!list->is_circular || cur->next != list->head))
            printf(" -> ");
        cur = cur->next; cnt++;
    } while (cur && cnt < max_display && (!list->is_circular || cur != list->head));
    if (list->is_circular && cnt >= max_display) printf(" ... (circular continues)");
    printf("\n");
}
static DllErrorCode free_list_safe(DoublyLinkedList *list) {
    if (!list) return DLL_ERROR_NULL_POINTER;
    if (!list->head) return DLL_SUCCESS;
    Node *cur = list->head; size_t freed_count = 0;
    if (list->is_circular && list->tail) { list->tail->next = NULL; list->head->prev = NULL; }
    while (cur) {
        Node *next = cur->next; free_node_safe(cur); cur = next; freed_count++;
        if (freed_count > list->size * 2) break; // safety
    }
    list->head = list->tail = NULL; list->size = 0; list->is_circular = false;
    return DLL_SUCCESS;
}

//--------------------
// Undo/Redo（応用アプリ層）
static DllErrorCode init_undo_redo_system(UndoRedoSystem *sys) {
    if (!sys) return DLL_ERROR_NULL_POINTER;
    DllErrorCode r;
    if ((r = init_list(&sys->undo_stack)) != DLL_SUCCESS) return r;
    if ((r = init_list(&sys->redo_stack)) != DLL_SUCCESS) return r;
    if ((r = init_list(&sys->main_data)) != DLL_SUCCESS) return r;
    return DLL_SUCCESS;
}
static DllErrorCode execute_operation(UndoRedoSystem *sys, int32_t data) {
    if (!sys) return DLL_ERROR_NULL_POINTER;
    DllErrorCode r;
    if ((r = stack_push(&sys->main_data, data)) != DLL_SUCCESS) return r;
    if ((r = stack_push(&sys->undo_stack, data)) != DLL_SUCCESS) return r;
    free_list_safe(&sys->redo_stack); init_list(&sys->redo_stack);
    printf("Operation executed: %d\n", data); return DLL_SUCCESS;
}
static DllErrorCode perform_undo(UndoRedoSystem *sys) {
    if (!sys) return DLL_ERROR_NULL_POINTER;
    int32_t undo_data, main_data; DllErrorCode r;
    if ((r = stack_pop(&sys->undo_stack, &undo_data)) != DLL_SUCCESS) { printf("No operation to undo\n"); return r; }
    if ((r = stack_pop(&sys->main_data, &main_data)) != DLL_SUCCESS) return r;
    if ((r = stack_push(&sys->redo_stack, main_data)) != DLL_SUCCESS) return r;
    printf("Undo executed: removed %d\n", main_data); return DLL_SUCCESS;
}
static DllErrorCode perform_redo(UndoRedoSystem *sys) {
    if (!sys) return DLL_ERROR_NULL_POINTER;
    int32_t redo_data; DllErrorCode r;
    if ((r = stack_pop(&sys->redo_stack, &redo_data)) != DLL_SUCCESS) { printf("No operation to redo\n"); return r; }
    if ((r = stack_push(&sys->main_data, redo_data)) != DLL_SUCCESS) return r;
    if ((r = stack_push(&sys->undo_stack, redo_data)) != DLL_SUCCESS) return r;
    printf("Redo executed: restored %d\n", redo_data); return DLL_SUCCESS;
}

//--------------------
// ラウンドロビン・スケジューラ（簡易）
static DllErrorCode simulate_round_robin(DoublyLinkedList *list, uint32_t time_slice, uint32_t max_cycles) {
    if (!list) return DLL_ERROR_NULL_POINTER;
    if (!list->head) { printf("No processes to schedule\n"); return DLL_ERROR_EMPTY_LIST; }
    if (!list->is_circular) { printf("Converting list to circular for round-robin\n"); make_circular(list); }
    printf("\n=== Round Robin Process Scheduler ===\n");
    printf("Time slice: %u seconds, Max cycles: %u\n", time_slice, max_cycles);
    printf("Processes: %zu\n\n", list->size);
    Node *current = list->head; uint32_t cycle = 0, total_time = 0;
    while (cycle < max_cycles) {
        printf("Cycle %u: Process %d executing for %u seconds\n", cycle + 1, current->data, time_slice);
        total_time += time_slice; current = current->next; cycle++;
        if (current == list->head && cycle % list->size == 0) printf("--- Completed one full round ---\n");
    }
    printf("\nScheduling completed: %u cycles, %u total seconds\n", cycle, total_time);
    return DLL_SUCCESS;
}

//--------------------
// 配列操作デモ
static DllErrorCode demonstrate_array_operations(void) {
    printf("\n=== Array Operations Demo ===\n");
    const size_t ARRAY_SIZE = 5;
    int32_t array[5] = {10, 20, 30, 40, 50};
    printf("Static array contents: ");
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        printf("%d", array[i]); if (i < ARRAY_SIZE - 1) printf(", ");
    }
    printf("\nRandom access: array[2] = %d\n", array[2]);
    printf("Array limitations: Fixed size (%zu elements)\n", ARRAY_SIZE);
    printf("Linked list advantages: Dynamic size, efficient insertion/deletion\n");
    return DLL_SUCCESS;
}

//--------------------
// テスト関数（責務分離＋ポインタ配列化）

static DllErrorCode test_stack_operations(void) {
    printf("\n=== Stack Operations Test ===\n");
    DoublyLinkedList stack; DllErrorCode r = init_list(&stack); if (r != DLL_SUCCESS) return r;
    printf("Pushing: 10, 20, 30\n");
    if ((r = stack_push(&stack, 10)) != DLL_SUCCESS) return r;
    if ((r = stack_push(&stack, 20)) != DLL_SUCCESS) return r;
    if ((r = stack_push(&stack, 30)) != DLL_SUCCESS) return r;
    print_list_safe(&stack, 10);
    int32_t val; printf("Popping:\n");
    while (stack.size > 0) { if ((r = stack_pop(&stack, &val)) == DLL_SUCCESS) printf("Popped: %d\n", val); }
    print_list_safe(&stack, 10);
    printf("Testing error case (pop from empty stack):\n");
    r = stack_pop(&stack, &val); if (r == DLL_ERROR_EMPTY_LIST) { printf("Correctly detected empty stack error\n"); r = DLL_SUCCESS; }
    free_list_safe(&stack); return r;
}
static DllErrorCode test_queue_operations(void) {
    printf("\n=== Queue Operations Test ===\n");
    DoublyLinkedList queue; DllErrorCode r = init_list(&queue); if (r != DLL_SUCCESS) return r;
    printf("Enqueueing: 100, 200, 300\n");
    if ((r = queue_enqueue(&queue, 100)) != DLL_SUCCESS) return r;
    if ((r = queue_enqueue(&queue, 200)) != DLL_SUCCESS) return r;
    if ((r = queue_enqueue(&queue, 300)) != DLL_SUCCESS) return r;
    print_list_safe(&queue, 10);
    int32_t val; printf("Dequeuing:\n");
    while (queue.size > 0) { if ((r = queue_dequeue(&queue, &val)) == DLL_SUCCESS) printf("Dequeued: %d\n", val); }
    print_list_safe(&queue, 10); free_list_safe(&queue); return DLL_SUCCESS;
}
static DllErrorCode test_undo_redo_system(void) {
    printf("\n=== Undo/Redo System Test ===\n");
    UndoRedoSystem sys; DllErrorCode r = init_undo_redo_system(&sys); if (r != DLL_SUCCESS) return r;
    printf("Executing: 1, 2, 3\n"); execute_operation(&sys, 1); execute_operation(&sys, 2); execute_operation(&sys, 3);
    printf("Current data: "); print_list_safe(&sys.main_data, 10);
    printf("\nUndo operations:\n"); perform_undo(&sys); printf("After undo: "); print_list_safe(&sys.main_data, 10);
    perform_undo(&sys); printf("After undo: "); print_list_safe(&sys.main_data, 10);
    printf("\nRedo operations:\n"); perform_redo(&sys); printf("After redo: "); print_list_safe(&sys.main_data, 10);
    free_list_safe(&sys.main_data); free_list_safe(&sys.undo_stack); free_list_safe(&sys.redo_stack);
    return DLL_SUCCESS;
}
static DllErrorCode test_circular_and_round_robin(void) {
    printf("\n=== Circular List & Round Robin Test ===\n");
    DoublyLinkedList plist; DllErrorCode r = init_list(&plist); if (r != DLL_SUCCESS) return r;
    printf("Adding processes: P1, P2, P3, P4\n");
    if ((r = queue_enqueue(&plist, 1)) != DLL_SUCCESS) return r;
    if ((r = queue_enqueue(&plist, 2)) != DLL_SUCCESS) return r;
    if ((r = queue_enqueue(&plist, 3)) != DLL_SUCCESS) return r;
    if ((r = queue_enqueue(&plist, 4)) != DLL_SUCCESS) return r;
    printf("Before making circular: "); print_list_safe(&plist, 10);
    r = simulate_round_robin(&plist, 2, 12);
    printf("After round robin: "); print_list_safe(&plist, 10);
    free_list_safe(&plist); return r;
}

//--------------------
// メイン
// ここからmain関数以降を完全にコピペでOK

int main(void) {
    printf("=================================================\n");
    printf("Production-Ready Doubly Linked List System\n");
    printf("Version: 1.0.0\nBuild: %s %s\n", __DATE__, __TIME__);
    printf("=================================================\n");

    // ログ出力例
    // log_debug("Application started: build=%s %s", __DATE__, __TIME__);

    DllErrorCode overall_result = DLL_SUCCESS;

    struct {
        const char *test_name;
        DllErrorCode (*test_func)(void);
    } test_suite[] = {
        {"Array Operations Demo", demonstrate_array_operations},
        {"Stack Operations", test_stack_operations},
        {"Queue Operations", test_queue_operations},
        {"Undo/Redo System", test_undo_redo_system},
        {"Circular List & Round Robin", test_circular_and_round_robin}
    };

    size_t num_tests = sizeof(test_suite) / sizeof(test_suite[0]);
    size_t passed_tests = 0;

    for (size_t i = 0; i < num_tests; i++) {
        printf("\n[TEST %zu/%zu] %s\n----------------------------------------\n", i + 1, num_tests, test_suite[i].test_name);
        DllErrorCode r = test_suite[i].test_func();
        if (r == DLL_SUCCESS) {
            printf("\xE2\x9C\x85 PASSED: %s\n", test_suite[i].test_name);
            passed_tests++;
        } else {
            printf("\xE2\x9D\x8C FAILED: %s (Error code: %d)\n", test_suite[i].test_name, r);
            overall_result = r;
        }
    }

    printf("\n=================================================\n");
    printf("TEST SUMMARY\n");
    printf("=================================================\n");
    printf("Total tests: %zu\n", num_tests);
    printf("Passed: %zu\n", passed_tests);
    printf("Failed: %zu\n", num_tests - passed_tests);

    if (overall_result == DLL_SUCCESS) {
        printf("\xF0\x9F\x8E\x89 ALL TESTS PASSED\n");
    } else {
        printf("\xF0\x9F\x92\xA5 SOME TESTS FAILED\n");
    }

    // メモリ使用状況レポート
    printf("\n=== Memory Usage Report ===\n");
    log_memory_stats();

    if (g_total_nodes_created != g_total_nodes_freed) {
        printf("\xE2\x9A\xA0\xEF\xB8\x8F  MEMORY LEAK DETECTED!\n");
        printf("Please check memory management implementation\n");
        overall_result = DLL_ERROR_MEMORY_ALLOC;
    } else {
        printf("\xE2\x9C\x85 No memory leaks detected\n");
    }

    // 本番環境での監視情報出力
    printf("\n=== Production Monitoring Info ===\n");
    printf("Timestamp: %ld\n", (long)time(NULL));
    printf("Process ID: %d\n", getpid());
    printf("Exit code: %d\n", overall_result == DLL_SUCCESS ? 0 : 1);

    if (overall_result == DLL_SUCCESS) {
        printf("STATUS=SUCCESS\n");
    } else {
        printf("STATUS=ERROR\n");
        fprintf(stderr, "Application terminated with errors\n");
    }

    printf("\n=== Application Terminated ===\n");
    return overall_result == DLL_SUCCESS ? 0 : 1;
}

// ↓（mainより前）に必要な関数・グローバル変数・型は必ず宣言・定義しておくこと！
>>>>>>> feature
