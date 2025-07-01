#include <stdio.h>
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "data_structures.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

// ---------- エラー・ログ定義 ----------
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
    DS_ERR_EMPTY_CONTAINER,
    DS_ERR_SYSTEM_FAILURE
} ds_error_t;

typedef enum {
    DS_LOG_DEBUG = 0, DS_LOG_INFO, DS_LOG_WARN, DS_LOG_ERROR, DS_LOG_FATAL
} ds_log_level_t;

typedef void (*ds_log_func_t)(ds_log_level_t level, const char* fmt, ...);

// ---------- メモリアロケータ型定義 ----------
typedef void* (*ds_malloc_func_t)(size_t);
typedef void  (*ds_free_func_t)(void*);

// ---------- 共通ユーティリティ ----------
void ds_set_log_function(ds_log_func_t func);
void ds_log(ds_log_level_t level, const char* fmt, ...);

// ---------- スタック ----------
typedef struct ds_stack ds_stack_t;
ds_stack_t* ds_stack_create(void);
ds_error_t ds_stack_destroy(ds_stack_t* stack);
ds_error_t ds_stack_push(ds_stack_t* stack, void* data);
ds_error_t ds_stack_pop(ds_stack_t* stack, void** out_data);
ds_error_t ds_stack_peek(const ds_stack_t* stack, void** out_data);
bool ds_stack_is_empty(const ds_stack_t* stack);
size_t ds_stack_size(const ds_stack_t* stack);
ds_error_t ds_stack_reset(ds_stack_t* stack);

// ---------- キュー ----------
typedef struct ds_queue ds_queue_t;
ds_queue_t* ds_queue_create(void);
ds_error_t ds_queue_destroy(ds_queue_t* queue);
ds_error_t ds_queue_enqueue(ds_queue_t* queue, void* data);
ds_error_t ds_queue_dequeue(ds_queue_t* queue, void** data);
ds_error_t ds_queue_front(const ds_queue_t* queue, void** data);
bool ds_queue_is_empty(const ds_queue_t* queue);
size_t ds_queue_size(const ds_queue_t* queue);

// ---------- 円環リスト ----------
typedef struct ds_circular_node ds_circular_node_t;
typedef struct ds_circular_list ds_circular_list_t;
ds_circular_list_t* ds_circular_list_create(void);
ds_error_t ds_circular_list_destroy(ds_circular_list_t* list);
ds_error_t ds_circular_list_insert(ds_circular_list_t* list, void* data);
ds_error_t ds_circular_list_remove(ds_circular_list_t* list, void** data);
bool ds_circular_list_is_empty(const ds_circular_list_t* list);

// ---------- ヒストリー管理（Undo/Redo） ----------
typedef struct ds_command ds_command_t;
typedef struct ds_history_system ds_history_system_t;
ds_history_system_t* ds_history_system_create(size_t max_history);
ds_error_t ds_history_system_destroy(ds_history_system_t* history);
ds_error_t ds_history_system_execute_command(ds_history_system_t* history, const ds_command_t* command);
ds_error_t ds_history_system_undo(ds_history_system_t* history);
ds_error_t ds_history_system_redo(ds_history_system_t* history);
bool ds_history_system_can_undo(const ds_history_system_t* history);
bool ds_history_system_can_redo(const ds_history_system_t* history);
ds_error_t ds_history_system_clear(ds_history_system_t* history);

// ---------- LRUキャッシュ ----------
typedef struct ds_lru_cache ds_lru_cache_t;
ds_lru_cache_t* ds_lru_cache_create(size_t capacity);
ds_error_t ds_lru_cache_destroy(ds_lru_cache_t* cache);
ds_error_t ds_lru_cache_get(ds_lru_cache_t* cache, int key, int* value);
ds_error_t ds_lru_cache_put(ds_lru_cache_t* cache, int key, int value);

// ---------- RPN電卓 ----------
typedef struct ds_rpn_calculator ds_rpn_calculator_t;
ds_rpn_calculator_t* ds_rpn_calculator_create(void);
ds_error_t ds_rpn_calculator_destroy(ds_rpn_calculator_t* calc);
ds_error_t ds_rpn_calculator_reset(ds_rpn_calculator_t* calc);
ds_error_t ds_rpn_calculator_push(ds_rpn_calculator_t* calc, double value);
ds_error_t ds_rpn_calculator_pop(ds_rpn_calculator_t* calc, double* out);
ds_error_t ds_rpn_calculator_evaluate(ds_rpn_calculator_t* calc, const char* expression, double* result);

// ---------- ラウンドロビンスケジューラ ----------
typedef struct ds_process ds_process_t;
typedef struct ds_round_robin_scheduler ds_round_robin_scheduler_t;
ds_round_robin_scheduler_t* ds_round_robin_scheduler_create(int time_quantum);
ds_error_t ds_round_robin_scheduler_destroy(ds_round_robin_scheduler_t* scheduler);
ds_error_t ds_round_robin_scheduler_add_process(ds_round_robin_scheduler_t* scheduler, const ds_process_t* process);
ds_error_t ds_round_robin_scheduler_get_next_process(ds_round_robin_scheduler_t* scheduler, ds_process_t* process_out);
ds_error_t ds_round_robin_scheduler_complete_process(ds_round_robin_scheduler_t* scheduler, int process_id);

// ---------- ハッシュマップ ----------
typedef struct ds_hashmap ds_hashmap_t;
ds_hashmap_t* ds_hashmap_create(void);
ds_error_t ds_hashmap_destroy(ds_hashmap_t* map);
ds_error_t ds_hashmap_put(ds_hashmap_t* map, const char* key, void* value);
ds_error_t ds_hashmap_get(ds_hashmap_t* map, const char* key, void** value);
ds_error_t ds_hashmap_remove(ds_hashmap_t* map, const char* key);

// ---------- URL短縮 ----------
typedef struct ds_url_shortener ds_url_shortener_t;
ds_url_shortener_t* ds_url_shortener_create(void);
ds_error_t ds_url_shortener_destroy(ds_url_shortener_t* us);
ds_error_t ds_url_shortener_shorten(ds_url_shortener_t* us, const char* url, char* out_short, size_t out_size);
ds_error_t ds_url_shortener_expand(ds_url_shortener_t* us, const char* short_url, char* out_url, size_t out_size);

// ---------- 統計ユーティリティ ----------
typedef struct ds_stats ds_stats_t;
ds_error_t ds_stats_reset(ds_stats_t* stats);
ds_error_t ds_stats_update(ds_stats_t* stats, double value);
double ds_stats_mean(const ds_stats_t* stats);
double ds_stats_median(const ds_stats_t* stats);
double ds_stats_mode(const ds_stats_t* stats);

// ---------- メモリ管理 ----------
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);

// ---------- その他 ----------
#define DS_UNUSED(x) ((void)(x))

#endif // DATA_STRUCTURES_H