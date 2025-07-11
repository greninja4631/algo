<<<<<<< HEAD
<<<<<<< HEAD:tests/test_doubly_list.c
#include "../include/ds/doubly_list.h"
=======
#include "data_structures.h"
>>>>>>> feature:tests/ds/test_doubly_list.c
#include <stdio.h>
#include <assert.h>
=======
/**
 * @file test_doubly_list.c
 * @brief ds_doubly_list_t モジュールの単体テスト
 * @details 本ファイルは test_main.c から呼び出すため main関数は持たない。
 */
>>>>>>> feature

#include "ds/doubly_list.h"
#include "util/test_util.h"
#include "util/logger.h"
#include <stdlib.h>

/* プロジェクト共通アサーションマクロ例 */
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

/**
 * @brief ds_doubly_list_t の基本機能テスト
 */
void ds_test_doubly_list_basic(void)
{
    ds_error_t err;
    ds_doubly_list_t* list = ds_doubly_list_create();
    void* out_data = NULL;
    int a = 10, b = 20, c = 30, d = 40;

    // --- 生成 ---
    DS_TEST_ASSERT(list != NULL, "create: not NULL");
    DS_TEST_ASSERT(ds_doubly_list_is_empty(list), "is_empty after create");
    DS_TEST_ASSERT(ds_doubly_list_size(list) == 0, "size after create");

    // --- 末尾追加 ---
    err = ds_doubly_list_insert_back(list, &a);
    DS_TEST_ASSERT(err == DS_SUCCESS, "insert_back a");
    err = ds_doubly_list_insert_back(list, &b);
    DS_TEST_ASSERT(err == DS_SUCCESS, "insert_back b");
    DS_TEST_ASSERT(ds_doubly_list_size(list) == 2, "size==2 after two insert_back");

    // --- 先頭追加 ---
    err = ds_doubly_list_insert_front(list, &c);
    DS_TEST_ASSERT(err == DS_SUCCESS, "insert_front c");
    DS_TEST_ASSERT(ds_doubly_list_size(list) == 3, "size==3 after insert_front");

    // --- 任意位置追加（1番目, 0-indexed）---
    err = ds_doubly_list_insert_at(list, 1, &d);
    DS_TEST_ASSERT(err == DS_SUCCESS, "insert_at index=1 d");
    DS_TEST_ASSERT(ds_doubly_list_size(list) == 4, "size==4 after insert_at");

    // --- get_atで要素取得 ---
    err = ds_doubly_list_get_at(list, 0, &out_data);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)out_data == c, "get_at 0: c");
    err = ds_doubly_list_get_at(list, 1, &out_data);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)out_data == d, "get_at 1: d");
    err = ds_doubly_list_get_at(list, 2, &out_data);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)out_data == a, "get_at 2: a");
    err = ds_doubly_list_get_at(list, 3, &out_data);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)out_data == b, "get_at 3: b");

    // --- remove_front ---
    err = ds_doubly_list_remove_front(list, &out_data);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)out_data == c, "remove_front: c");
    DS_TEST_ASSERT(ds_doubly_list_size(list) == 3, "size after remove_front");

    // --- remove_back ---
    err = ds_doubly_list_remove_back(list, &out_data);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)out_data == b, "remove_back: b");
    DS_TEST_ASSERT(ds_doubly_list_size(list) == 2, "size after remove_back");

    // --- remove_at（0-indexed: index=1, aを削除）---
    err = ds_doubly_list_remove_at(list, 1, &out_data);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)out_data == a, "remove_at 1: a");
    DS_TEST_ASSERT(ds_doubly_list_size(list) == 1, "size after remove_at");

    // --- remove_at（index=0, dを削除）---
    err = ds_doubly_list_remove_at(list, 0, &out_data);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)out_data == d, "remove_at 0: d");
    DS_TEST_ASSERT(ds_doubly_list_is_empty(list), "is_empty after all remove");

    // --- 空でremove ---
    err = ds_doubly_list_remove_front(list, &out_data);
    DS_TEST_ASSERT(err == DS_ERR_EMPTY, "remove_front: empty");

    // --- 破棄 ---
    err = ds_doubly_list_destroy(list);
    DS_TEST_ASSERT(err == DS_SUCCESS, "destroy: DS_SUCCESS");

    // --- NULL安全 ---
    DS_TEST_ASSERT(ds_doubly_list_is_empty(NULL), "is_empty: NULL");
    DS_TEST_ASSERT(ds_doubly_list_size(NULL) == 0, "size: NULL");
    DS_TEST_ASSERT(ds_doubly_list_destroy(NULL) == DS_ERR_NULL_POINTER, "destroy: NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_doubly_list_basic 完了");
}

/**
 * @brief ds_doubly_list_t のエッジケース・異常系・NULL安全テスト
 */
void ds_test_doubly_list_edge_cases(void)
{
    ds_error_t err;
    void* out_data = NULL;

    // --- NULL渡し ---
    err = ds_doubly_list_insert_front(NULL, &out_data);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "insert_front: NULL list");
    err = ds_doubly_list_insert_back(NULL, &out_data);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "insert_back: NULL list");
    err = ds_doubly_list_insert_at(NULL, 0, &out_data);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "insert_at: NULL list");

    err = ds_doubly_list_remove_front(NULL, &out_data);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "remove_front: NULL list");
    err = ds_doubly_list_remove_back(NULL, &out_data);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "remove_back: NULL list");
    err = ds_doubly_list_remove_at(NULL, 0, &out_data);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "remove_at: NULL list");

    err = ds_doubly_list_get_at(NULL, 0, &out_data);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "get_at: NULL list");

    err = ds_doubly_list_destroy(NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "destroy: NULL list");

    // --- index out of range ---
    ds_doubly_list_t* list = ds_doubly_list_create();
    DS_TEST_ASSERT(list != NULL, "create: for out-of-range test");

    err = ds_doubly_list_insert_back(list, &out_data);
    DS_TEST_ASSERT(err == DS_SUCCESS, "insert_back for out-of-range test");

    err = ds_doubly_list_get_at(list, 100, &out_data);
    DS_TEST_ASSERT(err == DS_ERR_OUT_OF_RANGE, "get_at: out of range");

    err = ds_doubly_list_remove_at(list, 100, &out_data);
    DS_TEST_ASSERT(err == DS_ERR_OUT_OF_RANGE, "remove_at: out of range");

    ds_doubly_list_destroy(list);

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_doubly_list_edge_cases 完了");
}

/**
 * @brief ds_doubly_list_t のメモリ・大量データテスト
 */
void ds_test_doubly_list_memory_management(void)
{
    ds_error_t err;
    ds_doubly_list_t* list = ds_doubly_list_create();
    int arr[256];
    void* out_data = NULL;

    for (size_t i = 0; i < 256; ++i) {
        arr[i] = (int)i;
        err = ds_doubly_list_insert_back(list, &arr[i]);
        DS_TEST_ASSERT(err == DS_SUCCESS, "memory: insert_back");
    }
    DS_TEST_ASSERT(ds_doubly_list_size(list) == 256, "memory: size==256");

    for (size_t i = 0; i < 256; ++i) {
        err = ds_doubly_list_remove_front(list, &out_data);
        DS_TEST_ASSERT(err == DS_SUCCESS, "memory: remove_front");
    }
    DS_TEST_ASSERT(ds_doubly_list_is_empty(list), "memory: is_empty after all remove");

    err = ds_doubly_list_destroy(list);
    DS_TEST_ASSERT(err == DS_SUCCESS, "memory: destroy");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_doubly_list_memory_management 完了");
}