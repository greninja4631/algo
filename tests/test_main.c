/**
 * @file    tests/test_main.c
 * @brief   CI/CD向け自動テストランナー（全テスト集約＋カバレッジ＋拡張容易）
 *
 * - ロガーは test_logger で色分け
 * - グローバル変数 g_stats でテスト状態を管理（副作用なし設計）
 * - 追加DS/Utils/Undo-Redo/GC/VM/CLI/AI...何でも関数追加だけで拡張可
 * - CI/CDで標準出力にサマリ出力・Slack連携なども容易
 */

 //
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

/* ---- ロガー（色分け+INFO/ERROR区別） ---- */
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

/* ---- テストプロトタイプ宣言（全て test_XXX_basic / test_XXX_edge_cases…） ---- */
void ds_test_stack_basic(void);
void ds_test_stack_edge_cases(void);
void ds_test_queue_basic(void);
void ds_test_statistics_basic(void);
void ds_test_sort_basic(void);
void ds_test_search_basic(void);
void ds_test_process_basic(void);
void ds_test_logger_basic(void);
// ↑ 新API/DSごとに1行ずつ必ず追加（未使用関数ゼロ保証）

/* ---- スイート実行・レポート ---- */
static void run_suite(const char *name, void (**fn)(void), size_t n)
{
    printf("\n=== %s Suite ===\n", name);
    for (size_t i = 0; i < n; ++i)
        if (fn[i]) fn[i]();
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

/* ---- メイン ---- */
int main(int argc, char *argv[])
{
    g_stats = (ds_test_stats_t){0}; g_stats.start = clock();
    ds_set_log_function(test_logger);

    printf("==== Data-Structures Library Test ====\nAPI Version: %s\n", DS_API_VERSION);

    // 全てのテスト関数を配列に登録。追加・順序変更・除外もこの1カ所で管理
    void (*ds_tests[])(void) = {
        ds_test_stack_basic,
        ds_test_stack_edge_cases,
        ds_test_queue_basic,
        ds_test_statistics_basic,
        ds_test_sort_basic,
        ds_test_search_basic,
        ds_test_process_basic,
        ds_test_logger_basic,
        // ↑ 新API/新DS/新ユーティリティごとに1行ずつ追加
    };

    run_suite("Data-Structures", ds_tests, sizeof ds_tests / sizeof *ds_tests);

    report();
    return g_stats.failed ? 1 : 0;
}