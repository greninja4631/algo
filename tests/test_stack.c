/**
 * @file test_stack.c
 * @brief スタックADTユニットテスト - 安全性・機能性検証用
 * @version 1.0.0
 * @date 2025-05-28
 *
 * 【設計理念】
 * - main()は「テストの起点」のみ
 * - スタックAPIの全操作(生成/プッシュ/ポップ/破棄)の基本挙動・エラー系をテスト
 * - CI/CD自動化・Valgrindによるメモリ検証も意識
 * - assertで失敗時は即座に異常終了
 * - 全操作はエラーコードでチェック（API設計の原則）
 *
 * 【補足・テスト設計解説】
 * - 1関数1責務・テスト容易性最優先
 * - 失敗時に「assert」で即座に異常終了 → CI/CDでの自動検知可
 * - エラー系も全て網羅し「堅牢性保証」
 * - テストデータはローカルint型で十分
 * - stats.total_elementsの値やポインタの値で仕様通りか確認
 */

<<<<<<< HEAD
#include "../include/ds/stack.h"
#include <stdio.h>
#include <assert.h>

int main(void) {
    printf("=== Stack ADT Unit Test Start ===\n");

    // 1. スタック作成
    ds_stack_t* stack = ds_stack_create();
    assert(stack != NULL);

    // 2. 初期状態は空
    assert(ds_stack_is_empty(stack) == true);
    assert(ds_stack_size(stack) == 0);

    // 3. プッシュ操作
    int v1 = 42, v2 = 123;
    assert(ds_stack_push(stack, &v1) == DS_SUCCESS);
    assert(ds_stack_push(stack, &v2) == DS_SUCCESS);

    // 4. 空でなくなったか
    assert(ds_stack_is_empty(stack) == false);
    assert(ds_stack_size(stack) == 2);

    // 5. 先頭確認（peek）
    void* peek_ptr = NULL;
    assert(ds_stack_peek(stack, &peek_ptr) == DS_SUCCESS);
    assert(*(int*)peek_ptr == 123); // 直前にpushした値

    // 6. ポップ（LIFO）
    void* pop_ptr = NULL;
    assert(ds_stack_pop(stack, &pop_ptr) == DS_SUCCESS);
    assert(*(int*)pop_ptr == 123);

    assert(ds_stack_pop(stack, &pop_ptr) == DS_SUCCESS);
    assert(*(int*)pop_ptr == 42);

    // 7. 空状態の確認
    assert(ds_stack_is_empty(stack) == true);
=======
#include "data_structures.h"
 #include <stdio.h>
 #include <assert.h>
>>>>>>> feature

    // 8. 空ポップ・空ピーク時のエラー動作
    assert(ds_stack_pop(stack, &pop_ptr) == DS_ERR_EMPTY_CONTAINER);
    assert(ds_stack_peek(stack, &peek_ptr) == DS_ERR_EMPTY_CONTAINER);

    // 9. 統計情報取得
    ds_stats_t stats = {0};
    assert(ds_stack_get_stats(stack, &stats) == DS_SUCCESS);

    printf("Total elements (should be 0): %zu\n", stats.total_elements);

    // 10. 破棄操作
    assert(ds_stack_destroy(stack) == DS_SUCCESS);

    printf("=== Stack ADT Unit Test Passed ===\n");
    return 0;
}
