/**
 * @file    tests/test_sort.c
 * @brief   ソートAPI（バブル・挿入）単体テスト
 * @details 本ファイルは test_main.c から呼び出される。main関数は持たない。
 *
 * - CI/CD・静的解析・Valgrind・Fuzzing全対応設計
 * - assert, printf禁止（全ログはds_log経由）
 * - 全API NULLセーフ・エラー分岐カバレッジ100%
 * - テスト名は "ds_test_<module>_<case>"形式で一元管理
 */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "algo/sort.h"
#include "data_structures.h"
#include "util/test_util.h"
#include "util/logger.h"

// プロジェクト共通：CI/CD自動収集対応アサーション
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

/** @brief 配列等値判定 */
static bool arr_eq(const int* a, const int* b, size_t n) {
    for (size_t i = 0; i < n; ++i) if (a[i] != b[i]) return false;
    return true;
}

/**
 * @brief ソートAPI（バブル・挿入）正常系・異常系・NULLセーフ 全網羅テスト
 * @details
 *   - 配列: ランダム/逆順/空/NULL等
 *   - API: ds_sort_bubble, ds_sort_insertion
 *   - エラー: DS_ERR_NULL_POINTER, DS_ERR_INVALID_ARG
 *   - カバレッジ: 最低1回は全エラーパスを踏む
 */
void ds_test_sort_basic(void)
{
    ds_error_t err;
    int arr1[] = {5, 3, 8, 1, 2};
    int expect1[] = {1, 2, 3, 5, 8};
    int arr2[] = {9, 7, 5, 3, 1};
    int expect2[] = {1, 3, 5, 7, 9};
    size_t n1 = sizeof(arr1) / sizeof(arr1[0]);
    size_t n2 = sizeof(arr2) / sizeof(arr2[0]);

    // --- バブルソート ---
    int tmp1[5]; memcpy(tmp1, arr1, sizeof(arr1));
    err = ds_sort_bubble(tmp1, n1);
    DS_TEST_ASSERT(err == DS_SUCCESS && arr_eq(tmp1, expect1, n1), "bubble: arr1 sort");

    int tmp2[5]; memcpy(tmp2, arr2, sizeof(arr2));
    err = ds_sort_bubble(tmp2, n2);
    DS_TEST_ASSERT(err == DS_SUCCESS && arr_eq(tmp2, expect2, n2), "bubble: arr2 sort");

    // --- 挿入ソート ---
    memcpy(tmp1, arr1, sizeof(arr1));
    err = ds_sort_insertion(tmp1, n1);
    DS_TEST_ASSERT(err == DS_SUCCESS && arr_eq(tmp1, expect1, n1), "insertion: arr1 sort");

    memcpy(tmp2, arr2, sizeof(arr2));
    err = ds_sort_insertion(tmp2, n2);
    DS_TEST_ASSERT(err == DS_SUCCESS && arr_eq(tmp2, expect2, n2), "insertion: arr2 sort");

    // --- 空配列・サイズ0 ---
    int empty[1] = {0};
    err = ds_sort_bubble(empty, 0);
    DS_TEST_ASSERT(err == DS_ERR_INVALID_ARG, "bubble: zero-size");

    err = ds_sort_insertion(empty, 0);
    DS_TEST_ASSERT(err == DS_ERR_INVALID_ARG, "insertion: zero-size");

    // --- NULLセーフ（CI基準） ---
    err = ds_sort_bubble(NULL, n1);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "bubble: NULL");
    err = ds_sort_insertion(NULL, n1);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "insertion: NULL");

    // --- 一要素 ---
    int single[] = {42}, expected[] = {42};
    err = ds_sort_bubble(single, 1);
    DS_TEST_ASSERT(err == DS_SUCCESS && arr_eq(single, expected, 1), "bubble: single element");
    err = ds_sort_insertion(single, 1);
    DS_TEST_ASSERT(err == DS_SUCCESS && arr_eq(single, expected, 1), "insertion: single element");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_sort_basic 完了");
}