#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- エラーコード定義 ---
typedef enum {
    DS_SUCCESS = 0,              // 正常終了
    DS_ERR_NULL_POINTER,         // NULLポインタ参照
    DS_ERR_ALLOC,                // メモリ確保失敗
    DS_ERR_EMPTY,                // 空の構造体
    DS_ERR_INVALID_ARG,          // 不正な引数
    DS_ERR_OVERFLOW,             // オーバーフロー
    DS_ERR_UNDERFLOW,            // アンダーフロー
    DS_ERR_NOT_FOUND,            // 要素未発見
    DS_ERR_SYSTEM,               // システムエラー
    DS_ERR_OUT_OF_MEMORY,        // メモリ不足
    DS_ERR_EMPTY_CONTAINER,      // 空pop/dequeue等でのエラー
    DS_ERR_SYSTEM_FAILURE        // Undo/Redo等での致命的失敗
} ds_error_t;

// 別名マクロ（現場での可読性向上目的）
#define DS_ERROR_NULL_POINTER      DS_ERR_NULL_POINTER
#define DS_ERROR_NOT_FOUND         DS_ERR_NOT_FOUND
#define DS_ERROR_EMPTY_CONTAINER   DS_ERR_EMPTY_CONTAINER
#define DS_ERROR_INVALID_ARGUMENT  DS_ERR_INVALID_ARG

const char* ds_error_string(ds_error_t error); // エラー文字列

// --- RPN電卓（逆ポーランド記法計算機）の型とAPI ---
typedef struct ds_rpn_calculator ds_rpn_calculator_t;
ds_rpn_calculator_t* ds_rpn_calculator_create(void);
ds_error_t ds_rpn_calculator_destroy(ds_rpn_calculator_t*);
ds_error_t ds_rpn_calculator_evaluate(ds_rpn_calculator_t*, const char* expr, double* result);
ds_error_t ds_rpn_calculator_reset(ds_rpn_calculator_t*);
ds_error_t ds_rpn_calculator_push(ds_rpn_calculator_t* calc, double value); // 内部スタックへ
ds_error_t ds_rpn_calculator_pop(ds_rpn_calculator_t* calc, double* out);

// --- 統計データ型 ---
typedef struct ds_stats {
    size_t total_elements;
    size_t memory_allocated;
    size_t operations_count;
    uint64_t creation_timestamp;
} ds_stats_t;

// --- Undo/Redo用コマンドパターン型 ---
typedef ds_error_t (*ds_command_fn_t)(void* data);
typedef struct {
    ds_command_fn_t execute;
    ds_command_fn_t undo;
    void* data;
} ds_command_t;

// --- Stack（スタック: LIFO）API宣言 ---
typedef struct ds_stack ds_stack_t;
ds_stack_t* ds_stack_create(void);
ds_error_t ds_stack_destroy(ds_stack_t*);
ds_error_t ds_stack_push(ds_stack_t*, void* data);
ds_error_t ds_stack_pop(ds_stack_t*, void** data);
ds_error_t ds_stack_peek(const ds_stack_t*, void** data);
bool ds_stack_is_empty(const ds_stack_t*);
size_t ds_stack_size(const ds_stack_t*);
ds_error_t ds_stack_get_stats(const ds_stack_t*, ds_stats_t*);
ds_error_t ds_stack_reset(ds_stack_t* stack);

// --- Queue（キュー: FIFO）API宣言 ---
typedef struct ds_queue ds_queue_t;
ds_queue_t* ds_queue_create(void);
ds_error_t ds_queue_destroy(ds_queue_t*);
ds_error_t ds_queue_enqueue(ds_queue_t*, void* data);
ds_error_t ds_queue_dequeue(ds_queue_t*, void** data);
bool ds_queue_is_empty(const ds_queue_t*);
size_t ds_queue_size(const ds_queue_t*);
ds_error_t ds_queue_get_stats(const ds_queue_t*, ds_stats_t*);
ds_error_t ds_queue_front(const ds_queue_t*, void** data);

// --- Doubly List（双方向リスト）API（不完全型のみ宣言） ---
typedef struct ds_doubly_list ds_doubly_list_t;

// --- Circular List Node（循環リストノード）---
typedef struct ds_circular_node {
    void* data;
    struct ds_circular_node* next;
} ds_circular_node_t;

// --- Circular List API ---
typedef struct ds_circular_list ds_circular_list_t;
ds_circular_list_t* ds_circular_list_create(void);
ds_error_t ds_circular_list_destroy(ds_circular_list_t* list);
ds_error_t ds_circular_list_insert(ds_circular_list_t* list, void* data);      // 末尾に追加
ds_error_t ds_circular_list_remove(ds_circular_list_t* list, void** data);     // 先頭から削除
bool       ds_circular_list_is_empty(const ds_circular_list_t* list);
size_t     ds_circular_list_size(const ds_circular_list_t* list);

// --- Undo/Redo履歴システムAPI ---
typedef struct ds_history_system ds_history_system_t;
ds_history_system_t* ds_history_system_create(size_t max_history);
ds_error_t ds_history_system_destroy(ds_history_system_t*);

// --- OS/スケジューラAPI: ラウンドロビン ---
typedef struct ds_process {
    int process_id;
    char name[32];
    int priority;
    int cpu_time;
    int memory_size;
    int remaining_time;
} ds_process_t;

typedef struct ds_round_robin_scheduler ds_round_robin_scheduler_t;
ds_round_robin_scheduler_t* ds_round_robin_scheduler_create(int time_quantum);
ds_error_t ds_round_robin_scheduler_destroy(ds_round_robin_scheduler_t*);
ds_error_t ds_round_robin_scheduler_add_process(ds_round_robin_scheduler_t*, const ds_process_t*);
// ※【テスト観点】ID重複不可とするなら、その旨コメントを追加
//   ds_round_robin_scheduler_add_process: process_idが既に存在する場合はDS_ERR_INVALID_ARGを返す
ds_error_t ds_round_robin_scheduler_get_next_process(ds_round_robin_scheduler_t*, ds_process_t*);
ds_error_t ds_round_robin_scheduler_complete_process(ds_round_robin_scheduler_t*, int process_id);

// --- ログ機能: 任意のロガーに切り替え可能 ---
typedef enum {
    DS_LOG_DEBUG = 0, DS_LOG_INFO, DS_LOG_WARN, DS_LOG_ERROR, DS_LOG_FATAL
} ds_log_level_t;
typedef void (*ds_log_func_t)(ds_log_level_t level, const char* fmt, ...);
void ds_set_log_function(ds_log_func_t func);

// --- メモリ管理: malloc/free差し替え可能 ---
typedef void* (*ds_malloc_func_t)(size_t size);
typedef void  (*ds_free_func_t)(void* ptr);
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);

#ifdef DS_UNIT_TEST
// ユニットテスト専用APIなど（例：内部構造へのアクセス用getterを限定的に宣言）
#endif

#ifdef __cplusplus
}
#endif
#endif /* DATA_STRUCTURES_H */