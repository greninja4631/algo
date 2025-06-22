#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "data_structures.h"
#include <stdarg.h>
#include <stdio.h>

// ----- エラーコード -----
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

// ----- メモリ・ロギング -----
typedef void* (*ds_malloc_func_t)(size_t);
typedef void  (*ds_free_func_t)(void*);
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);

typedef enum {
    DS_LOG_DEBUG = 0, DS_LOG_INFO, DS_LOG_WARN, DS_LOG_ERROR, DS_LOG_FATAL
} ds_log_level_t;
typedef void (*ds_log_func_t)(ds_log_level_t, const char*, ...);
void ds_set_log_function(ds_log_func_t func);
void ds_log(ds_log_level_t level, const char* fmt, ...);

// ----- スタック -----
typedef struct ds_stack ds_stack_t;
ds_stack_t* ds_stack_create(void);
ds_error_t ds_stack_destroy(ds_stack_t*);
ds_error_t ds_stack_push(ds_stack_t*, void*);
ds_error_t ds_stack_pop(ds_stack_t*, void**);
ds_error_t ds_stack_peek(const ds_stack_t*, void**);
bool ds_stack_is_empty(const ds_stack_t*);
size_t ds_stack_size(const ds_stack_t*);
ds_error_t ds_stack_get_stats(const ds_stack_t*, void*);
ds_error_t ds_stack_reset(ds_stack_t*);

// ----- キュー -----
typedef struct ds_queue ds_queue_t;
ds_queue_t* ds_queue_create(void);
ds_error_t ds_queue_destroy(ds_queue_t*);
ds_error_t ds_queue_enqueue(ds_queue_t*, void*);
ds_error_t ds_queue_dequeue(ds_queue_t*, void**);
bool ds_queue_is_empty(const ds_queue_t*);
size_t ds_queue_size(const ds_queue_t*);
ds_error_t ds_queue_get_stats(const ds_queue_t*, void*);
ds_error_t ds_queue_front(const ds_queue_t*, void**);

// ----- Doubly List -----
typedef struct ds_doubly_list ds_doubly_list_t;

// ----- Circular List -----
typedef struct ds_circular_node ds_circular_node_t;
typedef struct ds_circular_list ds_circular_list_t;

// ----- Undo/Redo履歴 -----
typedef struct ds_command ds_command_t;
typedef struct ds_history_system ds_history_system_t;

// ----- RPN電卓 -----
typedef struct ds_rpn_calculator ds_rpn_calculator_t;
ds_rpn_calculator_t* ds_rpn_calculator_create(void);
ds_error_t ds_rpn_calculator_destroy(ds_rpn_calculator_t*);
ds_error_t ds_rpn_calculator_evaluate(ds_rpn_calculator_t*, const char*, double*);
ds_error_t ds_rpn_calculator_reset(ds_rpn_calculator_t*);
ds_error_t ds_rpn_calculator_push(ds_rpn_calculator_t*, double);
ds_error_t ds_rpn_calculator_pop(ds_rpn_calculator_t*, double*);

// ----- ラウンドロビン -----
typedef struct ds_process ds_process_t;
typedef struct ds_round_robin_scheduler ds_round_robin_scheduler_t;

// ----- 汎用Stats -----
typedef struct ds_stats ds_stats_t;

#endif /* DATA_STRUCTURES_H */