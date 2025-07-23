#ifndef DS_DATA_STRUCTURES_H
#define DS_DATA_STRUCTURES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/**
 * @file    data_structures.h
 * @brief   全モジュール共通：エラーコード・ログレベル・型宣言・DIアロケータ等を一元集約
 * @note    typedef/enum/struct/opaque宣言は必ず本ファイルのみに集約・再宣言禁止
 * @note    変更はABI破壊を伴うためPRレビュー必須
 */

#define DS_API_VERSION "1.0.0"

#ifdef __cplusplus
extern "C" {
#endif

/*─────────────────────────────────────*
 * 1. 全体統一エラーコード
 *─────────────────────────────────────*/
typedef enum {
    DS_SUCCESS = 0,             /* 成功 */
    DS_ERR_NULL_POINTER,        /* NULL参照 */
    DS_ERR_ALLOC,               /* メモリ確保失敗 */
    DS_ERR_EMPTY,               /* 空構造体/バッファ */
    DS_ERR_INVALID_ARG,         /* 無効な引数 */
    DS_ERR_OUT_OF_RANGE,        /* 範囲外アクセス */
    DS_ERR_OVERFLOW,            /* オーバーフロー */
    DS_ERR_UNDERFLOW,           /* アンダーフロー */
    DS_ERR_NOT_FOUND,           /* データ未発見 */
    DS_ERR_SYSTEM,              /* OS/システムエラー */
    DS_ERR_IO,                  /* 入出力エラー */
    DS_ERR_TIMEOUT,             /* タイムアウト */
    DS_ERR_CONFLICT,            /* 競合・排他違反 */
    DS_ERR_NOT_IMPLEMENTED,     /* 未実装 */
    DS_ERR_UNSUPPORTED,         /* サポート外 */
    DS_ERR_PERMISSION,          /* 権限エラー */
    DS_ERR_FATAL,               /* 致命的エラー */
    DS_ERR_UNKNOWN              /* 不明なエラー */
} ds_error_t;

/*─────────────────────────────────────*
 * 2. ログレベル（小さいほど重大）
 *─────────────────────────────────────*/
typedef enum {
    DS_LOG_LEVEL_FATAL = 0,     /* 強制終了級 */
    DS_LOG_LEVEL_ERROR,         /* 重大エラー */
    DS_LOG_LEVEL_WARN,          /* 警告 */
    DS_LOG_LEVEL_INFO,          /* 通常情報 */
    DS_LOG_LEVEL_DEBUG,         /* デバッグ */
    DS_LOG_LEVEL_TRACE          /* トレース詳細 */
} ds_log_level_t;
/* 後方互換マクロ */
#define DS_LOG_ERROR DS_LOG_LEVEL_ERROR

/*─────────────────────────────────────*
 * 3. DI用ロガー関数型
 *─────────────────────────────────────*/
typedef void (*ds_log_func_t)(ds_log_level_t level, const char *fmt, va_list args);

/*─────────────────────────────────────*
 * 4. Opaque型/前方宣言一元管理
 *─────────────────────────────────────*/
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
typedef struct ds_dbconn                 ds_dbconn_t;
typedef struct ds_fuzz_context           ds_fuzz_context_t;
typedef struct ds_security_context       ds_security_context_t;

/*─────────────────────────────────────*
 * 5. DIアロケータ型・切替API
 *─────────────────────────────────────*/
/* 外部注入用メモリ関数型 */
typedef void* (*ds_malloc_func_t)(size_t);
typedef void  (*ds_free_func_t)(void*);

void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);

/**
 * @struct ds_allocator
 * @brief  DI用Opaqueアロケータ（calloc互換推奨。free必須。Arena/Poolも差し替え可能）
 * @note   @ownership "caller frees" or "transfer"
 * @note   NULL・0バイト時の動作も仕様化必須
 */
struct ds_allocator {
    void* (*alloc)(size_t count, size_t size);  /* calloc互換 */
    void  (*free)(void* ptr);                   /* free互換 */
    /* 必要に応じてrealloc/stats/owner/trace等を拡張可 */
};

/*─────────────────────────────────────*
 * 6. 汎用統計構造体
 *─────────────────────────────────────*/
typedef struct {
    size_t total_elements;
    size_t memory_allocated;
    size_t operations_count;
    size_t creation_timestamp;
    size_t error_count;
} ds_stats_t;

/*─────────────────────────────────────*
 * 7. コマンドパターン/Undo-Redo型
 *─────────────────────────────────────*/
typedef ds_error_t (*ds_command_exec_func_t)(void*);
typedef ds_error_t (*ds_command_undo_func_t)(void*);

typedef struct ds_command {
    ds_command_exec_func_t execute;
    ds_command_undo_func_t undo;
    void*                  ctx;
    /* undo/redoチェーンやIDも拡張可 */
} ds_command_t;

/*─────────────────────────────────────*
 * 8. 汎用設定・DTO例
 *─────────────────────────────────────*/
typedef struct {
    char   name[64];
    size_t value;
    int    enabled;
} ds_config_entry_t;

#ifdef __cplusplus
}
#endif
#endif // DS_DATA_STRUCTURES_H
