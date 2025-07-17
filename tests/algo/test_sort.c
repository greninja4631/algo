/**
 * @file test_sort.c
 * @brief ソートAPI（バブル・挿入）単体テスト
 * @details 本ファイルは test_main.c から呼び出される。main関数は持たない。
 */

#include "algo/sort.h"
#include "util/test_util.h"
#include "util/logger.h"
#include <string.h> // memcpy用

// テスト用アサーションマクロ（プロジェクト統一ルール）
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

/** @brief 配列の等値判定ユーティリティ */
static bool arr_eq(const int* a, const int* b, size_t n) {
    for (size_t i = 0; i < n; ++i) if (a[i] != b[i]) return false;
    return true;
}

void ds_test_sort_basic(void)
{
    ds_error_t err;
    int arr1[] = {5, 3, 8, 1, 2};
    int expect[] = {1, 2, 3, 5, 8};
    int arr2[] = {9, 7, 5, 3, 1};
    int expect2[] = {1, 3, 5, 7, 9};
    size_t n1 = sizeof(arr1) / sizeof(arr1[0]);
    size_t n2 = sizeof(arr2) / sizeof(arr2[0]);

    // --- バブルソート ---
    int tmp1[5]; memcpy(tmp1, arr1, sizeof(arr1));
    err = ds_sort_bubble(tmp1, n1);
    DS_TEST_ASSERT(err == DS_SUCCESS && arr_eq(tmp1, expect, n1), "bubble: arr1 sort");

    int tmp2[5]; memcpy(tmp2, arr2, sizeof(arr2));
    err = ds_sort_bubble(tmp2, n2);
    DS_TEST_ASSERT(err == DS_SUCCESS && arr_eq(tmp2, expect2, n2), "bubble: arr2 sort");

    // --- 挿入ソート ---
    memcpy(tmp1, arr1, sizeof(arr1));
    err = ds_sort_insertion(tmp1, n1);
    DS_TEST_ASSERT(err == DS_SUCCESS && arr_eq(tmp1, expect, n1), "insertion: arr1 sort");

    memcpy(tmp2, arr2, sizeof(arr2));
    err = ds_sort_insertion(tmp2, n2);
    DS_TEST_ASSERT(err == DS_SUCCESS && arr_eq(tmp2, expect2, n2), "insertion: arr2 sort");

    // --- 空配列 ---
    int empty[1] = {0};
    err = ds_sort_bubble(empty, 0);
    DS_TEST_ASSERT(err == DS_ERR_INVALID_ARG, "bubble: zero-size");

    err = ds_sort_insertion(empty, 0);
    DS_TEST_ASSERT(err == DS_ERR_INVALID_ARG, "insertion: zero-size");

    // --- NULL安全 ---
    err = ds_sort_bubble(NULL, n1);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "bubble: NULL");
    err = ds_sort_insertion(NULL, n1);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "insertion: NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_sort_basic 完了");
}
