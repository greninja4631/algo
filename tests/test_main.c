/**
 * @file    tests/test_main.c
 * @brief   CI/CD向け自動テストランナー（全テスト1発集約＋カバレッジ＋拡張容易）
 *
 * - ロガーはtest_loggerで色分け＋main.cとは別
 * - g_statsで状態管理。グローバル変数/副作用ゼロ推奨
 * - 形式手法／再帰定義／Undo-Redo／各DSごとのテスト全追加OK
 * - 結果は標準出力（CIでログ収集/Slack通知など容易）
 */

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "data_structures.h"
#include "util/logger.h"
#include "util/test_util.h"

/* ---- テスト統計 ---- */
typedef struct {
    size_t total, passed, failed, skipped;
    clock_t start, end;
} ds_test_stats_t;
static ds_test_stats_t g_stats;

/* ---- ロガー ---- */
static void test_logger(ds_log_level_t lvl, const char *fmt, va_list ap)
{
    static const char *L[] = { "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };
    printf("[TEST-%s] ", L[lvl]);
    vprintf(fmt, ap); putchar('\n');
}

/* ---- 共通ASSERT ---- */
#define DS_TEST_ASSERT(cond, msg) do {                                  \
    g_stats.total++;                                                    \
    if (cond) { g_stats.passed++; printf("[PASS] %s\n", msg); }         \
    else { g_stats.failed++; printf("[FAIL] %s (%s:%d)\n", msg, __FILE__, __LINE__); } \
} while (0)

/* ---- テストプロトタイプ宣言（全DS,全Utils,Undo-Redo,GC,VM,CLI,AI…） ---- */
void ds_test_stack_basic(void);
void ds_test_stack_edge_cases(void);
// ...他全て test_XXX_case で追加

static void run_suite(const char *name, void (**fn)(void), size_t n)
{
    printf("\n=== %s Suite ===\n", name);
    for (size_t i = 0; i < n; ++i) if (fn[i]) fn[i]();
}
static void report(void)
{
    g_stats.end = clock();
    double sec = (double)(g_stats.end - g_stats.start) / CLOCKS_PER_SEC;
    printf("\n=== Test Summary ===\n");
    printf("Total: %zu  Passed: %zu  Failed: %zu  Skipped: %zu\n",
           g_stats.total, g_stats.passed, g_stats.failed, g_stats.skipped);
    printf("Success Rate: %.1f%%   Time: %.2fs\n",
           g_stats.total ? 100.0 * g_stats.passed / g_stats.total : 0.0, sec);
}
int main(int argc, char *argv[])
{
    g_stats = (ds_test_stats_t){0}; g_stats.start = clock();
    ds_set_log_function(test_logger);

    printf("==== Data-Structures Library Test ====\nAPI Version: %s\n", DS_API_VERSION);

    void (*ds_tests[])(void) = {
        ds_test_stack_basic,
        ds_test_stack_edge_cases,
        // ...他の全テスト
    };

    run_suite("Data-Structures", ds_tests, sizeof ds_tests / sizeof *ds_tests);
    // 他モジュール（undo-redo, GC, VM, CLI, AI, etc）はここにどんどんsuite追加

    report();
    return g_stats.failed ? 1 : 0;
}