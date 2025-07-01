#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    DS_LOG_DEBUG = 0,
    DS_LOG_INFO,
    DS_LOG_WARN,
    DS_LOG_ERROR,
    DS_LOG_FATAL
} ds_log_level_t;

// ロガー関数ポインタ型
typedef void (*ds_log_func_t)(ds_log_level_t level, const char* fmt, ...);

// ロガーAPI
void ds_set_log_function(ds_log_func_t func);
void ds_log(ds_log_level_t level, const char* fmt, ...);

#endif // LOGGER_H
