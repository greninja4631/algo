#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "data_structures.h"
#include "util/logger.h"
#include "util/test_util.h"

/* ───────── テスト統計 ───────── */
typedef struct {
    size_t  total, passed, failed, skipped;
    clock_t start, end;
} ds_test_stats_t;

static ds_test_stats_t g_stats;

/* ───────── テスト用ロガー ───────── */
static void test_logger(ds_log_level_t lvl, const char *fmt, va_list ap)
{
    static const char *L[] = { "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };
    printf("[TEST-%s] ", L[lvl]);
    vprintf(fmt, ap);
    putchar('\n');
}

/* ───────── 共通 ASSERT ───────── */
#define DS_TEST_ASSERT(cond, msg) do {                                  \
        g_stats.total++;                                                \
        if (cond) {                                                     \
            g_stats.passed++;                                           \
            printf("[PASS] %s\n", msg);                                 \
        } else {                                                        \
            g_stats.failed++;                                           \
            printf("[FAIL] %s (%s:%d)\n", msg, __FILE__, __LINE__);     \
        }                                                               \
    } while (0)

/* ==== テスト関数プロトタイプ === */
/* DS モジュール */
void ds_test_stack_basic(void);
void ds_test_stack_edge_cases(void);
void test__next_next_practice_basic(void);
void test__next_next_practice_edge_cases(void);

/* Util モジュール */
void test__config_basic(void);
void test__config_edge_cases(void);
void test__memory_basic(void);
void test__memory_edge_cases(void);
void test__metrics_basic(void);
void ds_test_logger_basic(void);
void ds_test_logger_edge_cases(void);

/* ────────── スイート実行 ───────── */
static void run_suite(const char *name, void (**fn)(void), size_t n)
{
    printf("\n=== %s Suite ===\n", name);
    for (size_t i = 0; i < n; ++i) {
        if (fn[i]) fn[i]();
    }
}

/* ────────── レポート ───────── */
static void report(void)
{
    g_stats.end = clock();
    double sec = (double)(g_stats.end - g_stats.start) / CLOCKS_PER_SEC;

    printf("\n=== Test Summary ===\n");
    printf("Total: %zu  Passed: %zu  Failed: %zu  Skipped: %zu\n",
           g_stats.total, g_stats.passed, g_stats.failed, g_stats.skipped);
    printf("Success Rate: %.1f%%   Time: %.2fs\n",
           g_stats.total ? 100.0 * g_stats.passed / g_stats.total : 0.0,
           sec);
}

/* ────────── main() ───────── */
int main(int argc, char *argv[])
{
    g_stats = (ds_test_stats_t){0};
    g_stats.start = clock();

    ds_set_log_function(test_logger);
    printf("==== Data-Structures Library Test ====\n");
    printf("API Version: %s\n", DS_API_VERSION);

    /* DS モジュールのテスト群 */
    void (*ds_tests[])(void) = {
        ds_test_stack_basic,
        ds_test_stack_edge_cases,
        test__next_next_practice_basic,
        test__next_next_practice_edge_cases
    };

    /* Util モジュールのテスト群 */
    void (*util_tests[])(void) = {
        test__config_basic,
        test__config_edge_cases,
        test__memory_basic,
        test__memory_edge_cases,
        test__metrics_basic,
        ds_test_logger_basic,
        ds_test_logger_edge_cases
    };

    run_suite("Data-Structures", ds_tests,  sizeof ds_tests  / sizeof *ds_tests);
    run_suite("Utilities",      util_tests, sizeof util_tests / sizeof *util_tests);

    report();
    return g_stats.failed ? 1 : 0;
}