/**
 * @file  tests/ds/test_statistics.c
 * @brief statistics モジュールの単体テスト
 */
#include "statistics.h"
#include "util/test_util.h"
#include <assert.h>

/* ---- ログ付きアサート ---- */
#define TEST_ASSERT(cond, msg)                                              \
    do {                                                                    \
        if (cond) { LOG_INFO("[PASS] %s\n", msg); }                         \
        else      { LOG_ERROR("[FAIL] %s (%s:%d)\n",                        \
                               msg, __FILE__, __LINE__); assert(cond); }    \
    } while (0)

/* 基本ケース */
void test_statistics_basic(void)
{
    int arr[] = {5,1,7,1,2,7,7};
    int n = (int)(sizeof arr / sizeof arr[0]);

    TEST_ASSERT(sum(arr,n)     == 30,        "sum");
    TEST_ASSERT(min(arr,n)     == 1,         "min");
    TEST_ASSERT(max(arr,n)     == 7,         "max");
    TEST_ASSERT(average(arr,n) == 30.0/n,    "average");
    TEST_ASSERT(median(arr,n)  == 5,         "median");
    TEST_ASSERT(mode(arr,n)    == 7,         "mode");

    Statistics s = calculate_statistics(arr,n);
    TEST_ASSERT(s.sum == 30 && s.mode == 7,  "calculate_statistics");
}

/* エッジケース */
void test_statistics_edge_cases(void)
{
    /* 長さ 1 */
    int single[1] = {42};
    TEST_ASSERT(median(single,1) == 42, "median single");

    /* 長さ 0 : ISO C では空配列不可 → ダミー1要素配列 + n=0 */
    int dummy[1] = {0};
    int n = 0;
    (void)sum(dummy, n);   /* 仕様に応じた戻り値を後で検証可 */
}

/* 不正入力ケース */
void test_statistics_invalid_input(void)
{
    int *nullarr = NULL;
    int n = 8;
    (void)sum(nullarr,n);
    (void)min(nullarr,n);
    (void)max(nullarr,n);
    (void)average(nullarr,n);
    (void)median(nullarr,n);
    (void)mode(nullarr,n);
    (void)calculate_statistics(nullarr,n);
}