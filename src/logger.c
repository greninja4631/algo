#include "util/logger.h"
#include <stdio.h>

/* staticグローバルで現在のログ関数（差し替え可） */
static ds_log_func_t current_logger = NULL;

/* ───── デフォルト実装 ───── */
static void default_log(ds_log_level_t level, const char* fmt, va_list args) {
    static const char* level_str[] = { "DEBUG", "INFO", "WARNING", "ERROR", "FATAL" };
    int idx = (int)level;
    if (idx < 0 || idx > DS_LOG_LEVEL_FATAL) idx = DS_LOG_LEVEL_DEBUG;
    fprintf(stderr, "[%s] ", level_str[idx]);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
}

/* ───── 差し替えAPI ───── */
void ds_set_log_function(ds_log_func_t func) {
    current_logger = func ? func : default_log;
}

/* ───── ログ出力API ───── */
void ds_log(ds_log_level_t level, const char* fmt, ...) {
    if (!current_logger) current_logger = default_log;
    va_list args;
    va_start(args, fmt);
    current_logger(level, fmt, args);
    va_end(args);
}