#ifndef DS_LOGGER_H
#define DS_LOGGER_H
#define DS_LOG_ERROR DS_LOG_LEVEL_ERROR


#include <stdarg.h>

/**
 * @file logger.h
 * @brief 共通ロガーAPI（全モジュール共通）
 * @details ログレベル・型・差し替えAPIを一元管理（再定義禁止）。
 */

/* ログレベルは必ず「DS_LOG_LEVEL_」で統一・enum値明示 */
typedef enum {
    DS_LOG_LEVEL_DEBUG = 0,
    DS_LOG_LEVEL_INFO,
    DS_LOG_LEVEL_WARNING,
    DS_LOG_LEVEL_ERROR,
    DS_LOG_LEVEL_FATAL
} ds_log_level_t;

/* ロガー関数ポインタ型 - va_listのみ許可 */
typedef void (*ds_log_func_t)(ds_log_level_t level, const char* fmt, va_list args);

/* ロガー関数の差し替えAPI */
void ds_set_log_function(ds_log_func_t func);

/* ログ出力API（内部で登録関数に転送） */
void ds_log(ds_log_level_t level, const char* fmt, ...);



#endif /* DS_LOGGER_H */
