/**
 * @file test_queue.c
 * @brief キューADTユニットテスト - 安全性・機能性検証用
 * @version 1.0.0
 * @date 2025-06-25
 *
 * - API名・型は実装(header)と完全一致
 * - 全操作は assert で堅牢性を担保
 * - テスト失敗時は即異常終了
 */
#include "../include/ds/queue.h"
#include <stdio.h>
#include <assert.h>

int main(void) {
    printf("=== Queue ADT Unit Test Start ===\n");

    // 1. キュー作成
    ds_queue_t* queue = ds_queue_create();
    assert(queue != NULL);

    // 2. 初期状態は空
    assert(ds_queue_is_empty(queue) == true);
    assert(ds_queue_size(queue) == 0);

    // 3. エンキュー操作
    int v1 = 100, v2 = 200, v3 = 300;
    assert(ds_queue_enqueue(queue, &v1) == DS_SUCCESS);
    assert(ds_queue_enqueue(queue, &v2) == DS_SUCCESS);
    assert(ds_queue_enqueue(queue, &v3) == DS_SUCCESS);

    // 4. 空でなくなったか
    assert(ds_queue_is_empty(queue) == false);
    assert(ds_queue_size(queue) == 3);

    // 5. 先頭確認
    void* front_ptr = NULL;
    assert(ds_queue_front(queue, &front_ptr) == DS_SUCCESS);
    assert(*(int*)front_ptr == 100);

    // 6. デキュー操作（FIFO）
    void* deq_ptr = NULL;
    assert(ds_queue_dequeue(queue, &deq_ptr) == DS_SUCCESS);
    assert(*(int*)deq_ptr == 100);
    assert(ds_queue_dequeue(queue, &deq_ptr) == DS_SUCCESS);
    assert(*(int*)deq_ptr == 200);
    assert(ds_queue_dequeue(queue, &deq_ptr) == DS_SUCCESS);
    assert(*(int*)deq_ptr == 300);

    // 7. 空状態の確認
    assert(ds_queue_is_empty(queue) == true);

    // 8. 空デキュー・空フロント時のエラー動作
    assert(ds_queue_dequeue(queue, &deq_ptr) == DS_ERR_EMPTY_CONTAINER);
    assert(ds_queue_front(queue, &front_ptr) == DS_ERR_EMPTY_CONTAINER);

    // 9. 統計情報取得
    ds_stats_t stats = {0};
    assert(ds_queue_get_stats(queue, &stats) == DS_SUCCESS);
    printf("Total elements (should be 0): %zu\n", stats.total_elements);

    // 10. 破棄操作
    assert(ds_queue_destroy(queue) == DS_SUCCESS);

    printf("=== Queue ADT Unit Test Passed ===\n");
    return 0;
}
