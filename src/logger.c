/**
 * @file   src/util/logger.c
 * @brief  共通ロガー実装（DI方式・typedef一切再宣言禁止！）
 */

#include "util/logger.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

/* 静的スコープ: 差し替え関数ポインタ */
static ds_log_func_t current_logger = NULL;

/* ログレベル文字列（enum順） */
static const char * const level_str[] = {
    "FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE"
};

/* デフォルトロガー実装 */
static void default_log(ds_log_level_t level, const char *fmt, va_list args) {
    int idx = (int)level;
    if (idx < 0 || idx > DS_LOG_LEVEL_TRACE) idx = DS_LOG_LEVEL_DEBUG;
    char buf[32];
    time_t now = time(NULL);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    fprintf(stderr, "[%s][%s] ", buf, level_str[idx]);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
}

/* DI差し替えAPI */
void ds_set_log_function(ds_log_func_t func) {
    current_logger = func ? func : default_log;
}

/* ログ出力API */
void ds_log(ds_log_level_t level, const char *fmt, ...) {
    if (!current_logger) current_logger = default_log;
    va_list args;
    va_start(args, fmt);
    current_logger(level, fmt, args);
    va_end(args);
}
