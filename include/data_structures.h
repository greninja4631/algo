/**
 * @file data_structures.h
 * @brief シリコンバレー標準準拠 汎用データ構造・OS風ADT宣言（クラウド観測性・CI/CD対応）
 * @version 1.2.3
 * @date 2025-05-28
 * @author Takumi Matsuda
 * @note
 *  - ADT責務分離・型安全・監視・国際化・将来拡張性・ユニットテスト容易性・クラウド監視/metrics.c/観測性・CI/CD対応
 */

 #ifndef DATA_STRUCTURES_H
 #define DATA_STRUCTURES_H
 
 #include <stddef.h>
 #include <stdbool.h>
 #include <stdint.h>
 #include "config.h"
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /* --- エラーコード（型安全・監視・CI/CD） --- */
 typedef enum {
    DS_SUCCESS = 0,
    DS_ERR_NULL_POINTER,
    DS_ERR_ALLOC,
    DS_ERR_EMPTY,
    DS_ERR_INVALID_ARG,
    DS_ERR_OVERFLOW,
    DS_ERR_UNDERFLOW,
    DS_ERR_NOT_FOUND,
    DS_ERR_SYSTEM,
    DS_ERR_OUT_OF_MEMORY,
    DS_ERR_EMPTY_CONTAINER,    ///< 空のコンテナ操作（pop/dequeue等）時の標準エラー
    DS_ERR_SYSTEM_FAILURE      ///< システムエラー/Undo/Redo/コマンド系で致命的失敗を明示
} ds_error_t;
// --- テスト互換性のためのエイリアス定義 ---
#define DS_ERROR_NULL_POINTER     DS_ERR_NULL_POINTER
#define DS_ERROR_NOT_FOUND       DS_ERR_NOT_FOUND
#define DS_ERROR_EMPTY_CONTAINER DS_ERR_EMPTY_CONTAINER
#define DS_ERROR_INVALID_ARGUMENT DS_ERR_INVALID_ARG

// --- RPN Calculator型/関数プロトタイプ ---
typedef struct ds_rpn_calculator ds_rpn_calculator_t;
ds_rpn_calculator_t* ds_rpn_calculator_create(void);
ds_error_t ds_rpn_calculator_destroy(ds_rpn_calculator_t* calc);
ds_error_t ds_rpn_calculator_evaluate(ds_rpn_calculator_t* calc, const char* expr, double* result);
ds_error_t ds_rpn_calculator_reset(ds_rpn_calculator_t* calc);

 const char* ds_error_string(ds_error_t error);
 
 /* --- 観測・統計データ型（全データ構造/metrics.c/クラウド監視連携/CI/CD） --- */
 typedef struct ds_stats {
     size_t total_elements;         ///< 現在の要素数（push, pop, insert, remove等で増減）
     size_t memory_allocated;       ///< ヒープ確保バイト数
     size_t operations_count;       ///< API呼び出し総数（push/pop/insert/remove等の累計）
     uint64_t creation_timestamp;   ///< 構造体生成時刻（UNIXタイム）
     // 拡張性：将来的な観測項目もここに追加可能
 } ds_stats_t;
 
 /* --- コマンドパターン用（Undo/Redo/History/CI） --- */
 typedef ds_error_t (*ds_command_fn_t)(void* data);
 
 typedef struct {
     ds_command_fn_t execute;
     ds_command_fn_t undo;
     void* data;
 } ds_command_t;
 
 /* --- Stack API（ADT） --- */
 typedef struct ds_stack ds_stack_t;
 ds_stack_t* ds_stack_create(void);
 ds_error_t ds_stack_destroy(ds_stack_t* stack);
 ds_error_t ds_stack_push(ds_stack_t* stack, void* data);
 ds_error_t ds_stack_pop(ds_stack_t* stack, void** data);
 ds_error_t ds_stack_peek(const ds_stack_t* stack, void** data);
 bool ds_stack_is_empty(const ds_stack_t* stack);
 size_t ds_stack_size(const ds_stack_t* stack);
 ds_error_t ds_stack_get_stats(const ds_stack_t* stack, ds_stats_t* stats);
 
 /* --- Queue API（ADT） --- */
 typedef struct ds_queue ds_queue_t;
 ds_queue_t* ds_queue_create(void);
 ds_error_t ds_queue_destroy(ds_queue_t* queue);
 ds_error_t ds_queue_enqueue(ds_queue_t* queue, void* data);
 ds_error_t ds_queue_dequeue(ds_queue_t* queue, void** data);
 bool ds_queue_is_empty(const ds_queue_t* queue);
 size_t ds_queue_size(const ds_queue_t* queue);
 ds_error_t ds_queue_get_stats(const ds_queue_t* queue, ds_stats_t* stats);
 ds_error_t ds_queue_front(const ds_queue_t* queue, void** data);  // 先頭参照API
 
 /* --- Doubly List API（ADT） --- */
 typedef struct ds_doubly_list ds_doubly_list_t;
 // 必要なAPIのみ宣言（拡張時に記載）
 
 /* --- RPN Calculator API --- */
 // ...ADT型宣言と関数群省略
 
 /* --- History System API（Undo/Redo） --- */
 typedef struct ds_history_system ds_history_system_t;
 ds_history_system_t* ds_history_system_create(size_t max_history);
 ds_error_t ds_history_system_destroy(ds_history_system_t* history);
 // ...追加API宣言可
 
 /* --- OS/スケジューラAPI（ラウンドロビン等） --- */
 /**
  * @brief プロセス構造体（クラウド/OS/テスト兼用・拡張容易設計）
  * - 現場テスト・拡張性に即対応
  */
 typedef struct ds_process {
     int process_id;
     char name[32];        ///< プロセス名
     int priority;         ///< 優先度
     int cpu_time;         ///< 割り当てCPU時間
     int memory_size;      ///< 必要メモリサイズ
     int remaining_time;   ///< 残り実行時間
     // 今後他のプロセスメタ情報もここに追記可
 } ds_process_t;
 
 typedef struct ds_round_robin_scheduler ds_round_robin_scheduler_t;
 ds_round_robin_scheduler_t* ds_round_robin_scheduler_create(int time_quantum);
 ds_error_t ds_round_robin_scheduler_destroy(ds_round_robin_scheduler_t* scheduler);
 ds_error_t ds_round_robin_scheduler_add_process(ds_round_robin_scheduler_t* scheduler, const ds_process_t* process);
 ds_error_t ds_round_robin_scheduler_get_next_process(ds_round_robin_scheduler_t* scheduler, ds_process_t* process_out);
 ds_error_t ds_round_robin_scheduler_complete_process(ds_round_robin_scheduler_t* scheduler, int process_id);
 // ...必要なAPI追記
 
 /* --- ログ・観測性（DevOps/クラウド用/CI） --- */
 typedef enum {
     DS_LOG_DEBUG = 0, DS_LOG_INFO, DS_LOG_WARN, DS_LOG_ERROR, DS_LOG_FATAL
 } ds_log_level_t;
 typedef void (*ds_log_func_t)(ds_log_level_t level, const char* fmt, ...);
 void ds_set_log_function(ds_log_func_t func);
 
 /* --- メモリ管理 --- */
 typedef void* (*ds_malloc_func_t)(size_t size);
 typedef void  (*ds_free_func_t)(void* ptr);
 void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);
 
 /* --- テストフック/CIフック --- */
 #ifdef DS_UNIT_TEST
 // ユニットテスト専用API（必要に応じて記述）
 #endif
 
 #ifdef __cplusplus
 }
 #endif
 #endif /* DATA_STRUCTURES_H */