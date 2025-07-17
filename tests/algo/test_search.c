/**
 * @file test_search.c
 * @brief 線形・二分探索APIの単体テスト
 * @details 本ファイルは test_main.c から呼び出される。main関数は持たない。
 */

#include "algo/search.h"
#include "util/test_util.h"
#include "util/logger.h"
#include <stddef.h>

// テスト用アサーションマクロ
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

void ds_test_search_basic(void)
{
    ds_error_t err;
    ptrdiff_t idx;
    int arr[] = {10, 20, 30, 40, 50};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    // --- 線形探索: ヒットケース ---
    err = ds_search_linear(arr, n, 30, &idx);
    DS_TEST_ASSERT(err == DS_SUCCESS && idx == 2, "linear: found 30 at index 2");

    // --- 線形探索: 先頭/末尾/失敗 ---
    err = ds_search_linear(arr, n, 10, &idx);
    DS_TEST_ASSERT(err == DS_SUCCESS && idx == 0, "linear: found 10 at index 0");
    err = ds_search_linear(arr, n, 50, &idx);
    DS_TEST_ASSERT(err == DS_SUCCESS && idx == 4, "linear: found 50 at index 4");
    err = ds_search_linear(arr, n, 99, &idx);
    DS_TEST_ASSERT(err == DS_ERR_NOT_FOUND && idx == -1, "linear: 99 not found");

    // --- 線形探索: NULL安全 ---
    err = ds_search_linear(NULL, n, 30, &idx);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "linear: arr NULL");
    err = ds_search_linear(arr, n, 30, NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "linear: out_index NULL");

    // --- 二分探索: ヒットケース ---
    err = ds_search_binary(arr, n, 20, &idx);
    DS_TEST_ASSERT(err == DS_SUCCESS && idx == 1, "binary: found 20 at index 1");

    // --- 二分探索: 失敗 ---
    err = ds_search_binary(arr, n, 60, &idx);
    DS_TEST_ASSERT(err == DS_ERR_NOT_FOUND && idx == -1, "binary: 60 not found");

    // --- 二分探索: NULL安全 ---
    err = ds_search_binary(NULL, n, 30, &idx);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "binary: arr NULL");
    err = ds_search_binary(arr, n, 30, NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "binary: out_index NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_search_basic 完了");
}
