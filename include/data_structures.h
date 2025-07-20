#ifndef DS_DATA_STRUCTURES_H
#define DS_DATA_STRUCTURES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/**
 * @file    data_structures.h
 * @brief   全モジュール共通の型宣言・エラーコード・Opaque型・DIアロケータ・共通構造体
 * @note    typedef/enum/前方宣言は本ファイル一元管理。重複定義・再宣言は禁止！
 * @note    @ownershipはAPIごとに明示、メモリ所有権の誤用・バグゼロを目指す。
 * @note    変更時はABI破壊を伴うためPRレビューフロー必須。
 */

#define DS_API_VERSION "1.0.0"

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD

=======
/* エラー型 */
>>>>>>> feature
=======
// --- エラー型（全モジュール共通） ---
>>>>>>> feature
=======
/*==================*
 * 共通エラー型宣言 *
 *==================*/
// --- 全モジュール統一エラーコード ---
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
    DS_ERR_TIMEOUT,
    DS_ERR_CONFLICT,
    DS_ERR_NOT_IMPLEMENTED,
    DS_ERR_UNSUPPORTED,
    DS_ERR_PERMISSION,
    DS_ERR_FATAL,
    DS_ERR_UNKNOWN
} ds_error_t;

<<<<<<< HEAD
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
=======
/*=====================*
 * 前方宣言・Opaque型  *
 *=====================*/
typedef struct ds_hashmap                ds_hashmap_t;
typedef struct ds_doubly_list            ds_doubly_list_t;
typedef struct ds_circular_list          ds_circular_list_t;
typedef struct ds_history_system         ds_history_system_t;
typedef struct ds_lru_cache              ds_lru_cache_t;
typedef struct ds_next_next_practice     ds_next_next_practice_t;
typedef struct ds_process                ds_process_t;
typedef struct ds_queue                  ds_queue_t;
typedef struct ds_round_robin_scheduler  ds_round_robin_scheduler_t;
typedef struct ds_rpn_calculator         ds_rpn_calculator_t;
typedef struct ds_stack                  ds_stack_t;
typedef struct ds_statistics             ds_statistics_t;
typedef struct ds_url_shortener          ds_url_shortener_t;
typedef struct ds_allocator              ds_allocator_t;
typedef struct ds_metrics                ds_metrics_t;
typedef struct ds_config                 ds_config_t;
typedef struct ds_logger                 ds_logger_t;
typedef struct ds_memory                 ds_memory_t;
typedef struct ds_dbconn                 ds_dbconn_t;      // DB/拡張用途
typedef struct ds_fuzz_context           ds_fuzz_context_t;// Fuzzing拡張
typedef struct ds_security_context       ds_security_context_t; // セキュリティ拡張
>>>>>>> feature

/*=====================*
 * DIアロケータ設計    *
 *=====================*/
// 外部注入用メモリ関数型
typedef void* (*ds_malloc_func_t)(size_t);
typedef void  (*ds_free_func_t)(void*);

// DIアロケータのグローバル差し替え
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);

/**
 * @struct ds_allocator
 * @brief  DI用Opaqueアロケータ（calloc互換推奨。free必須。Arena/Poolも差し替え可能）
 * @note   @ownership "caller frees" or "transfer"
 * @note   安全性確保のためNULL・0バイト時の動作を必ず仕様化。
 */
struct ds_allocator {
    void* (*alloc)(size_t count, size_t size);  // calloc互換
    void  (*free)(void* ptr);                   // free互換
    // （拡張用）必要に応じてrealloc, stats, owner, trace等を追加
};

/*=======================*
 * 汎用統計・メトリクス  *
 *=======================*/
typedef struct {
    size_t total_elements;       // 現在要素数
    size_t memory_allocated;     // 累計/現在アロケートバイト
    size_t operations_count;     // 操作回数
    size_t creation_timestamp;   // 作成時刻
    size_t error_count;          // エラー発生回数（拡張）
} ds_stats_t;

/*============================*
 * コマンドパターンAPI        *
 *============================*/
typedef ds_error_t (*ds_command_exec_func_t)(void*);
typedef ds_error_t (*ds_command_undo_func_t)(void*);

typedef struct ds_command {
    ds_command_exec_func_t execute;
    ds_command_undo_func_t undo;
    void*                  ctx;
    // undo/redoスタックをたどる場合はnext/prevやIDを追加
} ds_command_t;

/*==========================*
 * 共通コンフィグ構造体例   *
 *==========================*/
typedef struct {
    char   name[64];
    size_t value;
    int    enabled;
    // ... 必要に応じて拡張
} ds_config_entry_t;

#endif // DS_DATA_STRUCTURES_H