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
