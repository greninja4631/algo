/**
 * @file tests/test_main.c
 * @brief 全自動テストエントリーポイント
 * @details 各種 test_<module>_<case> 関数を収集・実行
 */

#include "data_structures.h"
#include "util/logger.h"
#include "util/test_util.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

/* テスト統計 */
typedef struct {
    size_t total, passed, failed, skipped;
    clock_t start, end;
} ds_test_stats_t;

static ds_test_stats_t g_test_stats = {0};

/* テスト専用ロガー */
static void test_logger(ds_log_level_t level, const char* fmt, va_list args) {
    const char* lvl[] = {"DEBUG","INFO","WARNING","ERROR","FATAL"};
    printf("[TEST-%s] ", lvl[level]);
    vprintf(fmt, args);
    printf("\n");
}

#define DS_UNUSED(x) ((void)(x))

/* テストassert */
#define DS_TEST_ASSERT(cond, msg) do { \
    g_test_stats.total++; \
    if (cond) { g_test_stats.passed++; printf("[PASS] %s\n", msg); } \
    else      { g_test_stats.failed++; printf("[FAIL] %s (%s:%d)\n", msg, __FILE__, __LINE__); } \
} while(0)

/* テスト関数プロトタイプ（別ファイルで実体実装） */
void test_stack_basic(void);
void test_stack_edge(void);
void test_config_basic(void);
void test_config_edge(void);
void test_logger_basic(void);
void test_logger_edge(void);
/* ...他、すべてtest_<module>_<case> */

/* テストスイート実行 */
static void run_suite(const char* name, void (**funcs)(void), size_t n) {
    printf("\n=== %s Suite ===\n", name);
    for (size_t i = 0; i < n; ++i) if (funcs[i]) funcs[i]();
}

/* テスト統計レポート */
static void report_stats(void) {
    g_test_stats.end = clock();
    double sec = (g_test_stats.end - g_test_stats.start) / (double)CLOCKS_PER_SEC;
    printf("\n=== Test Summary ===\n");
    printf("Total: %zu  Passed: %zu  Failed: %zu  Skipped: %zu\n", 
        g_test_stats.total, g_test_stats.passed, g_test_stats.failed, g_test_stats.skipped);
    printf("Success Rate: %.1f%%   Time: %.2fs\n",
        g_test_stats.total ? (100.0 * g_test_stats.passed / g_test_stats.total) : 0, sec);
    if (g_test_stats.failed) printf("[WARNING] Some tests failed!\n");
    else printf("[SUCCESS] All tests passed!\n");
}

/* main(): 唯一のテスト実行入口 */
int main(void) {
    g_test_stats = (ds_test_stats_t){0};
    g_test_stats.start = clock();
    ds_set_log_function(test_logger);
    printf("==== Data Structures Library Test ====\n");
    printf("API Version: %s\n", DS_API_VERSION);

    /* 各テスト配列。関数は外部実体 */
    void (*ds_tests[])(void) = { test_stack_basic, test_stack_edge };
    void (*util_tests[])(void) = { test_config_basic, test_config_edge, test_logger_basic, test_logger_edge };
    /* ...他 suite ... */

    run_suite("Data Structures", ds_tests, sizeof(ds_tests)/sizeof(ds_tests[0]));
    run_suite("Utilities", util_tests, sizeof(util_tests)/sizeof(util_tests[0]));
    /* ... */

    report_stats();
    return g_test_stats.failed ? 1 : 0;
}