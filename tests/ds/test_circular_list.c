<<<<<<< HEAD
<<<<<<< HEAD:tests/test_circular_list.c
/**
 * @file test_circular_list.c
 * @brief 円環リストADTユニットテスト
 * @version 1.0.0
 * @date 2025-06-25
 *
 * - API名は data_structures.h と完全一致
 * - 全操作に assert による安全性テスト
 * - 失敗時は即異常終了し、CIでも検知しやすい
 */

#include "../include/ds/circular_list.h"
=======
#include "data_structures.h"
>>>>>>> feature:tests/ds/test_circular_list.c
#include <stdio.h>
#include <assert.h>

int main(void) {
    printf("=== Circular List ADT Unit Test Start ===\n");

    // 1. リスト生成
    ds_circular_list_t* list = ds_circular_list_create();
    assert(list != NULL);

    // 2. 空判定
    assert(ds_circular_list_is_empty(list) == true);

    // 3. 挿入
    int v1 = 1, v2 = 2, v3 = 3;
    assert(ds_circular_list_insert(list, &v1) == DS_SUCCESS);
    assert(ds_circular_list_insert(list, &v2) == DS_SUCCESS);
    assert(ds_circular_list_insert(list, &v3) == DS_SUCCESS);

    // 4. 空でなくなったか
    assert(ds_circular_list_is_empty(list) == false);

    // 5. 要素を順番に取り出す（円環なので何回でもOK。今回は3回だけ）
    void* out = NULL;
    assert(ds_circular_list_remove(list, &out) == DS_SUCCESS);
    assert(*(int*)out == 1);

    assert(ds_circular_list_remove(list, &out) == DS_SUCCESS);
    assert(*(int*)out == 2);

    assert(ds_circular_list_remove(list, &out) == DS_SUCCESS);
    assert(*(int*)out == 3);

    // 6. 空状態
    assert(ds_circular_list_is_empty(list) == true);

    // 7. 空removeでエラー
    assert(ds_circular_list_remove(list, &out) == DS_ERR_EMPTY_CONTAINER);

    // 8. 破棄
    assert(ds_circular_list_destroy(list) == DS_SUCCESS);

    printf("=== Circular List ADT Unit Test Passed ===\n");
    return 0;
}
=======
/**
 * @file test_circular_list.c
 * @brief ds_circular_list_t モジュールの単体テスト
 * @details 本ファイルは test_main.c から呼び出される。main関数は持たない。
 */

#include "ds/circular_list.h"
#include "util/test_util.h"
#include "util/logger.h"

// テスト用アサーション（プロジェクト共通マクロ利用推奨）
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

// 基本動作テスト
void ds_test_circular_list_basic(void)
{
    ds_error_t err;
    ds_circular_list_t* list = NULL;
    void* data = NULL;
    int a = 10, b = 20, c = 30;

    // --- 生成 ---
    err = ds_circular_list_create(&list);
    DS_TEST_ASSERT(err == DS_SUCCESS, "create: DS_SUCCESS");
    DS_TEST_ASSERT(list != NULL, "create: not NULL");
    DS_TEST_ASSERT(ds_circular_list_is_empty(list), "is_empty: after create");
    DS_TEST_ASSERT(ds_circular_list_size(list) == 0, "size: after create");

    // --- 追加 ---
    err = ds_circular_list_insert(list, &a);
    DS_TEST_ASSERT(err == DS_SUCCESS, "insert a");
    DS_TEST_ASSERT(!ds_circular_list_is_empty(list), "not empty after insert");
    DS_TEST_ASSERT(ds_circular_list_size(list) == 1, "size 1");

    err = ds_circular_list_insert(list, &b);
    DS_TEST_ASSERT(err == DS_SUCCESS, "insert b");
    DS_TEST_ASSERT(ds_circular_list_size(list) == 2, "size 2");

    err = ds_circular_list_insert(list, &c);
    DS_TEST_ASSERT(err == DS_SUCCESS, "insert c");
    DS_TEST_ASSERT(ds_circular_list_size(list) == 3, "size 3");

    // --- 削除 ---
    err = ds_circular_list_remove(list, &data);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)data == a, "remove: a");
    DS_TEST_ASSERT(ds_circular_list_size(list) == 2, "size 2 after remove a");

    err = ds_circular_list_remove(list, &data);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)data == b, "remove: b");
    DS_TEST_ASSERT(ds_circular_list_size(list) == 1, "size 1 after remove b");

    err = ds_circular_list_remove(list, &data);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)data == c, "remove: c");
    DS_TEST_ASSERT(ds_circular_list_size(list) == 0, "size 0 after remove all");
    DS_TEST_ASSERT(ds_circular_list_is_empty(list), "is_empty after all removed");

    // --- 空でremove ---
    err = ds_circular_list_remove(list, &data);
    DS_TEST_ASSERT(err == DS_ERR_EMPTY, "remove: empty list");

    // --- 破棄 ---
    err = ds_circular_list_destroy(list);
    DS_TEST_ASSERT(err == DS_SUCCESS, "destroy: DS_SUCCESS");

    // --- NULL安全 ---
    DS_TEST_ASSERT(ds_circular_list_is_empty(NULL), "is_empty: NULL");
    DS_TEST_ASSERT(ds_circular_list_size(NULL) == 0, "size: NULL");
    DS_TEST_ASSERT(ds_circular_list_destroy(NULL) == DS_ERR_NULL_POINTER, "destroy: NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_circular_list_basic 完了");
}
>>>>>>> feature
