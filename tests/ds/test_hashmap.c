/**
 * @file test_hashmap.c
 * @brief ds_hashmap_t モジュールの単体テスト
 * @details 本ファイルは test_main.c から呼び出される。main関数は持たない。
 */

#include "ds/hashmap.h"
#include "util/logger.h"
#include "util/test_util.h"
#include <stdlib.h> 

// テスト用アサーションマクロ
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

static void ds_test_hashmap_free_dummy(void* ptr) {
    // 何もしないダミー
    (void)ptr;
}

void ds_test_hashmap_basic(void)
{
    ds_error_t err;
    ds_hashmap_t* map = NULL;
    void* value = NULL;
    const char* key1 = "apple";
    const char* key2 = "banana";
    const char* key3 = "orange";
    int v1 = 111, v2 = 222, v3 = 333;

    // --- 生成 ---
    err = ds_hashmap_create(16, ds_test_hashmap_free_dummy, ds_test_hashmap_free_dummy, &map);
    DS_TEST_ASSERT(err == DS_SUCCESS, "create: DS_SUCCESS");
    DS_TEST_ASSERT(map != NULL, "create: map not NULL");
    DS_TEST_ASSERT(ds_hashmap_size(map) == 0, "size: after create");

    // --- put/get ---
    err = ds_hashmap_put(map, key1, &v1);
    DS_TEST_ASSERT(err == DS_SUCCESS, "put: key1");
    err = ds_hashmap_put(map, key2, &v2);
    DS_TEST_ASSERT(err == DS_SUCCESS, "put: key2");
    err = ds_hashmap_put(map, key3, &v3);
    DS_TEST_ASSERT(err == DS_SUCCESS, "put: key3");
    DS_TEST_ASSERT(ds_hashmap_size(map) == 3, "size: after put 3 keys");

    err = ds_hashmap_get(map, key1, &value);
    DS_TEST_ASSERT(err == DS_SUCCESS && value == &v1, "get: key1");
    err = ds_hashmap_get(map, key2, &value);
    DS_TEST_ASSERT(err == DS_SUCCESS && value == &v2, "get: key2");
    err = ds_hashmap_get(map, key3, &value);
    DS_TEST_ASSERT(err == DS_SUCCESS && value == &v3, "get: key3");

    // --- remove ---
    err = ds_hashmap_remove(map, key2);
    DS_TEST_ASSERT(err == DS_SUCCESS, "remove: key2");
    DS_TEST_ASSERT(ds_hashmap_size(map) == 2, "size: after remove key2");

    // --- remove nonexistent ---
    err = ds_hashmap_remove(map, "not_exist");
    DS_TEST_ASSERT(err != DS_SUCCESS, "remove: not_exist (should fail)");

    // --- get after remove ---
    err = ds_hashmap_get(map, key2, &value);
    DS_TEST_ASSERT(err != DS_SUCCESS, "get: key2 after remove (should fail)");

    // --- NULL安全 ---
    err = ds_hashmap_put(NULL, key1, &v1);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "put: NULL map");
    err = ds_hashmap_get(NULL, key1, &value);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "get: NULL map");
    err = ds_hashmap_remove(NULL, key1);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "remove: NULL map");
    DS_TEST_ASSERT(ds_hashmap_size(NULL) == 0, "size: NULL map");

    // --- 破棄 ---
    err = ds_hashmap_destroy(map);
    DS_TEST_ASSERT(err == DS_SUCCESS, "destroy: DS_SUCCESS");
    DS_TEST_ASSERT(ds_hashmap_destroy(NULL) == DS_ERR_NULL_POINTER, "destroy: NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_hashmap_basic 完了");
}