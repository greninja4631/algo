#ifndef DS_DATA_STRUCTURES_H
#define DS_DATA_STRUCTURES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/**
 * @file    data_structures.h
 * @brief   全モジュール共通の型宣言・エラーコード・Opaque型・DIアロケータ・共通構造体
 * @note    typedef/enum/前方宣言は本ファイル一元管理。重複定義は絶対禁止！
 */

#define DS_API_VERSION "1.0.0"

<<<<<<< HEAD
<<<<<<< HEAD

=======
/* エラー型 */
>>>>>>> feature
=======
// --- エラー型（全モジュール共通） ---
>>>>>>> feature
typedef enum {
    DS_SUCCESS = 0,
    DS_ERR_NULL_POINTER,
    DS_ERR_ALLOC,
    DS_ERR_EMPTY,
    DS_ERR_INVALID_ARG,
    DS_ERR_OUT_OF_RANGE,
    DS_ERR_OVERFLOW,
    DS_ERR_UNDERFLOW,
    DS_ERR_NOT_FOUND,
    DS_ERR_SYSTEM,
    DS_ERR_IO,
    DS_ERR_UNKNOWN
} ds_error_t;

<<<<<<< HEAD
<<<<<<< HEAD
typedef enum {
    DS_LOG_DEBUG = 0,
    DS_LOG_INFO,
    DS_LOG_WARN,
    DS_LOG_ERROR,
    DS_LOG_FATAL
} ds_log_level_t;

typedef void (*ds_log_func_t)(ds_log_level_t level, const char* fmt, ...);

// ---------- メモリアロケータ型定義 ----------
typedef void* (*ds_malloc_func_t)(size_t);
typedef void  (*ds_free_func_t)(void*);

void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);

// ---------- 共通ユーティリティ・統計 ----------
typedef struct ds_stats {
    size_t   total_elements;
    size_t   memory_allocated;
    size_t   operations_count;
    uint64_t creation_timestamp;
=======
/* 統計型（本体もOK: 汎用/再定義なし前提） */
typedef struct {
    size_t total_elements;
    size_t memory_allocated;
    size_t operations_count;
    size_t creation_timestamp;
>>>>>>> feature
} ds_stats_t;

/* メモリ管理コールバック型 */
typedef void* (*ds_malloc_func_t)(size_t);
typedef void  (*ds_free_func_t)(void*);
void ds_set_memory_functions(ds_malloc_func_t, ds_free_func_t);

/* コマンドパターン共通型 */
typedef ds_error_t (*ds_command_exec_func_t)(void*);
typedef ds_error_t (*ds_command_undo_func_t)(void*);
typedef struct ds_command {
    ds_command_exec_func_t execute;
    ds_command_undo_func_t undo;
    void*                  ctx;
} ds_command_t;

=======
// --- Opaque型（前方宣言はここ1カ所のみ） ---
>>>>>>> feature
typedef struct ds_hashmap               ds_hashmap_t;
typedef struct ds_doubly_list           ds_doubly_list_t;
typedef struct ds_circular_list         ds_circular_list_t;
typedef struct ds_history_system        ds_history_system_t;
typedef struct ds_lru_cache             ds_lru_cache_t;
typedef struct ds_next_next_practice    ds_next_next_practice_t;
typedef struct ds_process               ds_process_t;
typedef struct ds_queue                 ds_queue_t;
typedef struct ds_round_robin_scheduler ds_round_robin_scheduler_t;
typedef struct ds_rpn_calculator        ds_rpn_calculator_t;
typedef struct ds_stack                 ds_stack_t;
typedef struct ds_statistics            ds_statistics_t;
typedef struct ds_url_shortener         ds_url_shortener_t;
typedef struct ds_allocator             ds_allocator_t;
typedef struct ds_metrics               ds_metrics_t;      // ←★ 必須: metrics_tの前方宣言
typedef struct ds_config                ds_config_t;
typedef struct ds_logger                ds_logger_t;
typedef struct ds_memory                ds_memory_t;

// --- メモリ管理・DIアロケータ ---
typedef void* (*ds_malloc_func_t)(size_t);
typedef void  (*ds_free_func_t)(void*);
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);

/**
 * @struct ds_allocator
 * @brief  DI用Opaqueアロケータ（calloc互換が基本。free必須）
 */
struct ds_allocator {
    void* (*alloc)(size_t count, size_t size);
    void  (*free)(void* ptr);
};

// --- 汎用統計構造体（他APIと共用可） ---
typedef struct {
    size_t total_elements;
    size_t memory_allocated;
    size_t operations_count;
    size_t creation_timestamp;
} ds_stats_t;

// --- コマンドパターン（Undo/Redo/Compositeで利用） ---
typedef ds_error_t (*ds_command_exec_func_t)(void*);
typedef ds_error_t (*ds_command_undo_func_t)(void*);
typedef struct ds_command {
    ds_command_exec_func_t execute;
    ds_command_undo_func_t undo;
    void*                  ctx;
} ds_command_t;

#endif // DS_DATA_STRUCTURES_H