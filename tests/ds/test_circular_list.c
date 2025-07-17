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
 * @file    tests/ds/test_circular_list.c
 * @brief   ds_circular_list モジュールの単体テスト
 * @details main() は tests/test_main.c で一括管理。
 */

#include <stdlib.h>             /* calloc / free */
#include "ds/circular_list.h"   /* 被テスト API */
#include "util/logger.h"        /* ds_log */

/*─────────────────────────────────────*
 * 1. 標準 malloc/free を ds_allocator_t
 *─────────────────────────────────────*/
static void* _std_alloc(size_t cnt, size_t size) { return calloc(cnt, size); }
static void  _std_free (void*  p)                { free(p);                }

static const ds_allocator_t g_alloc_impl = {
    .alloc = _std_alloc,
    .free  = _std_free
};
#define G_ALLOC (&g_alloc_impl)

/*───────────────────────────────*
 * 2. 共通アサーションマクロ
 *───────────────────────────────*/
#define TASSERT(cond, msg)                                                     \
    do {                                                                       \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO,  "[PASS] %s", (msg)); }          \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",            \
                           (msg), __FILE__, __LINE__); }                       \
    } while (0)

/*───────────────────────────────*
 * 3. 基本動作テスト
 *───────────────────────────────*/
void test__circular_list_basic(void)
{
    ds_circular_list_t *list = NULL;
    void *tmp = NULL;
    int a = 10, b = 20, c = 30;

    /* Create */
    TASSERT(ds_circular_list_create(G_ALLOC, &list) == DS_SUCCESS, "create");
    TASSERT(list != NULL,                                "list != NULL");
    TASSERT(ds_circular_list_is_empty(list),             "empty after create");
    TASSERT(ds_circular_list_size(list) == 0,            "size 0");

    /* Insert */
    TASSERT(ds_circular_list_insert(G_ALLOC, list, &a) == DS_SUCCESS, "insert a");
    TASSERT(ds_circular_list_insert(G_ALLOC, list, &b) == DS_SUCCESS, "insert b");
    TASSERT(ds_circular_list_insert(G_ALLOC, list, &c) == DS_SUCCESS, "insert c");
    TASSERT(ds_circular_list_size(list) == 3,                         "size 3");

    /* Remove (FIFO with rotation) */
    TASSERT(ds_circular_list_remove(G_ALLOC, list, &tmp) == DS_SUCCESS && *(int*)tmp == a,
            "remove a");
    TASSERT(ds_circular_list_remove(G_ALLOC, list, &tmp) == DS_SUCCESS && *(int*)tmp == b,
            "remove b");
    TASSERT(ds_circular_list_remove(G_ALLOC, list, &tmp) == DS_SUCCESS && *(int*)tmp == c,
            "remove c");
    TASSERT(ds_circular_list_is_empty(list), "empty after all remove");

    /* Remove on empty */
    TASSERT(ds_circular_list_remove(G_ALLOC, list, &tmp) == DS_ERR_EMPTY,
            "remove empty");

    /* Destroy */
    TASSERT(ds_circular_list_destroy(G_ALLOC, list) == DS_SUCCESS, "destroy OK");

    /* NULL-safety checks */
    TASSERT(ds_circular_list_is_empty(NULL),               "is_empty(NULL)");
    TASSERT(ds_circular_list_size(NULL) == 0,              "size(NULL)");
    TASSERT(ds_circular_list_destroy(G_ALLOC, NULL) == DS_ERR_NULL_POINTER,
            "destroy(NULL)");

<<<<<<< HEAD
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
=======
    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__circular_list_basic finished");
}
>>>>>>> feature
