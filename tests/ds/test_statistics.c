/**
 * @file test_statistics.c
 * @brief statistics.h の単体テスト（main無し・テストスイートから呼び出す想定）
 *
 * 統計APIの各関数が正しく動作することを検証。
 */

#include "statistics.h"
#include "util/test_util.h"
#include <assert.h>

// 標準化アサーションマクロ
#define TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { LOG_INFO("[PASS] %s\n", msg); } \
        else      { LOG_ERROR("[FAIL] %s (%s:%d)\n", msg, __FILE__, __LINE__); assert(cond); } \
    } while (0)

void test_statistics_basic(void)
{
    int arr[] = {5, 1, 7, 1, 2, 7, 7};
    int n = sizeof(arr) / sizeof(arr[0]);

    // sum
    TEST_ASSERT(sum(arr, n) == 30, "sum");

    // min
    TEST_ASSERT(min(arr, n) == 1, "min");

    // max
    TEST_ASSERT(max(arr, n) == 7, "max");

    // average
    TEST_ASSERT(average(arr, n) == (double)30 / n, "average");

    // median
    TEST_ASSERT(median(arr, n) == 5, "median");

    // mode
    TEST_ASSERT(mode(arr, n) == 7, "mode");

    // 全統計
    Statistics stat = calculate_statistics(arr, n);
    TEST_ASSERT(stat.sum == 30, "stat.sum");
    TEST_ASSERT(stat.min == 1, "stat.min");
    TEST_ASSERT(stat.max == 7, "stat.max");
    TEST_ASSERT(stat.average == (double)30 / n, "stat.average");
    TEST_ASSERT(stat.median == 5, "stat.median");
    TEST_ASSERT(stat.mode == 7, "stat.mode");

    LOG_INFO("[OK] test_statistics_basic 完了\n");
}

void test_statistics_edge_cases(void)
{
    // 長さ1
    int single[] = {42};
    int n = 1;
    TEST_ASSERT(sum(single, n) == 42, "sum/single");
    TEST_ASSERT(min(single, n) == 42, "min/single");
    TEST_ASSERT(max(single, n) == 42, "max/single");
    TEST_ASSERT(average(single, n) == 42.0, "average/single");
    TEST_ASSERT(median(single, n) == 42.0, "median/single");
    TEST_ASSERT(mode(single, n) == 42, "mode/single");

    // 長さ0（不正入力）
    int empty[] = {};
    n = 0;
    // すべてが定義通りのエラー値を返すこと（要実装と設計ポリシーに準拠してテスト）
    // 例: sum(empty, 0) == 0, min(empty, 0) == 0 など

    LOG_INFO("[OK] test_statistics_edge_cases 完了\n");
}

void test_statistics_invalid_input(void)
{
    // NULLポインタ・負数など
    int* nullarr = NULL;
    int n = 10;

    // 全関数が失敗値やガード処理で落ちないこと
    // ここは assert で落ちるのを避ける用途でテスト
    (void)sum(nullarr, n);
    (void)min(nullarr, n);
    (void)max(nullarr, n);
    (void)average(nullarr, n);
    (void)median(nullarr, n);
    (void)mode(nullarr, n);
    (void)calculate_statistics(nullarr, n);

    LOG_INFO("[OK] test_statistics_invalid_input 完了\n");
}