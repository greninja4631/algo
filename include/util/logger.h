#ifndef LOGGER_H
#define LOGGER_H

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

// ログレベル
typedef enum {
    DS_LOG_INFO,
    DS_LOG_WARN,
    DS_LOG_ERROR,
    DS_LOG_DEBUG
} ds_log_level_t;

// ログ関数型
typedef void (*ds_log_func_t)(ds_log_level_t level, const char* fmt, ...);

// グローバルログ関数の設定
void ds_set_log_function(ds_log_func_t func);

// デフォルトログ（標準出力）
void ds_log(ds_log_level_t level, const char* fmt, ...);

// ショートカットマクロ
#define DS_LOG_INFO  DS_LOG_INFO
#define DS_LOG_ERROR DS_LOG_ERROR

// エラーコードと変換
typedef enum {
    DS_SUCCESS = 0,
    DS_ERR_NULL_POINTER,
    DS_ERR_ALLOC,
    // ... 他のエラー
} ds_error_t;

const char* ds_error_string(ds_error_t err);

#endif // LOGGER_H
