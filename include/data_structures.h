#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

// ---------- 共通エラー・ログ型 ----------
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

const char* ds_error_string(ds_error_t err);

typedef enum {
    DS_LOG_DEBUG = 0,
    DS_LOG_INFO,
    DS_LOG_WARN,
    DS_LOG_ERROR,
    DS_LOG_FATAL
} ds_log_level_t;

typedef void (*ds_log_func_t)(ds_log_level_t level, const char* fmt, ...);

void ds_set_log_function(ds_log_func_t func);
void ds_log(ds_log_level_t level, const char* fmt, ...);

// ---------- 共通メモリアロケータ ----------
typedef void* (*ds_malloc_func_t)(size_t size);
typedef void  (*ds_free_func_t)(void* ptr);

void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);
void* ds_malloc(size_t size);
void  ds_free(void* ptr);

// ---------- 統計情報構造体 ----------
typedef struct {
    size_t total_elements;
    size_t memory_allocated;
    size_t operations_count;
    size_t creation_timestamp;
} ds_stats_t;

// コマンド構造体（Undo/Redo対応）
typedef int (*ds_command_exec_func)(void* data);  // ds_error_t型でもOK
typedef int (*ds_command_undo_func)(void* data);

typedef struct ds_command {
    ds_error_t (*execute)(void* data);
    ds_error_t (*undo)(void* data);
    void* data;
} ds_command_t;

// data_structures.h より
typedef struct ds_process {
    int process_id;
    char name[32];
    int priority;
    int burst_time;
    int arrival_time;
} ds_process_t;


// ---------- 汎用マクロ ----------
#define DS_UNUSED(x) ((void)(x))

#endif // DATA_STRUCTURES_H
