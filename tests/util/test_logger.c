#include <stdio.h>
/**
 * @file test_logger.c
 * @brief ロガーAPIのテスト（基礎・境界値）
 */

#include "util/logger.h"
#include <stdio.h>
#include <stdarg.h>

/* --- テスト用ロガー --- */
static void test_logger_capture(ds_log_level_t level, const char* fmt, va_list args) {
    const char* level_str = "";
    switch (level) {
        case DS_LOG_LEVEL_DEBUG:   level_str = "DEBUG"; break;
        case DS_LOG_LEVEL_INFO:    level_str = "INFO"; break;
        case DS_LOG_LEVEL_WARNING: level_str = "WARNING"; break;
        case DS_LOG_LEVEL_ERROR:   level_str = "ERROR"; break;
        case DS_LOG_LEVEL_FATAL:   level_str = "FATAL"; break;
        default:                   level_str = "UNKNOWN"; break;
    }
    printf("[TEST-LOGGER-%s] ", level_str);
    vprintf(fmt, args);
}

/* --- テスト本体（main不要。ds_test_logger_basicを呼ぶのはtest_main.c） --- */

void ds_test_logger_basic(void) {
    ds_set_log_function(test_logger_capture);

    ds_log(DS_LOG_LEVEL_INFO, "Logger info level: %d\n", 123);
    ds_log(DS_LOG_LEVEL_WARNING, "Logger warning: %s\n", "warn-msg");
    ds_log(DS_LOG_LEVEL_ERROR, "Logger error! code=%d\n", -1);

    // 実装が足りなければここにASSERTや追加ケース
    // DS_TEST_ASSERT... も追加可能
}

void ds_test_logger_edge_cases(void) {
    ds_set_log_function(test_logger_capture);

    ds_log(DS_LOG_LEVEL_DEBUG, "Debug log (edge)\n");
    ds_log(DS_LOG_LEVEL_FATAL, "Fatal log (edge case)\n");

    // ここにも異常系など追加
    // DS_TEST_ASSERT... も追加可能
}
