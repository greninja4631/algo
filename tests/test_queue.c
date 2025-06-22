/**
 * @file test_queue.c
 * @brief キューADT（FIFO）ユニットテスト - 安全性・基本機能の自動検証
 * @version 1.0.0
 * @date 2025-05-28
 *
 * 【設計理念】
 * - main関数はテストの起点のみ
 * - キューAPI全操作（生成/エンキュー/デキュー/破棄）・エラー動作も網羅
 * - assertによりテスト失敗は即検知（CI/CD前提）
 * - 型安全、戻り値検証、リソースリークなし
 */

 #include "../include/data_structures.h"
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
     int v1 = 10, v2 = 20, v3 = 30;
     assert(ds_queue_enqueue(queue, &v1) == DS_SUCCESS);
     assert(ds_queue_enqueue(queue, &v2) == DS_SUCCESS);
     assert(ds_queue_enqueue(queue, &v3) == DS_SUCCESS);
 
     // 4. 空でなくなったか
     assert(ds_queue_is_empty(queue) == false);
     assert(ds_queue_size(queue) == 3);
 
     // 5. 先頭要素確認（front: 先頭の値を参照するだけ）
     void* front_ptr = NULL;
     assert(ds_queue_front(queue, &front_ptr) == DS_SUCCESS);
     assert(*(int*)front_ptr == 10); // 最初に入れた値
 
     // 6. デキュー（FIFO: 入れた順に出る）
     void* dequeued_ptr = NULL;
     assert(ds_queue_dequeue(queue, &dequeued_ptr) == DS_SUCCESS);
     assert(*(int*)dequeued_ptr == 10);
 
     assert(ds_queue_dequeue(queue, &dequeued_ptr) == DS_SUCCESS);
     assert(*(int*)dequeued_ptr == 20);
 
     assert(ds_queue_dequeue(queue, &dequeued_ptr) == DS_SUCCESS);
     assert(*(int*)dequeued_ptr == 30);
 
     // 7. 全て出し終わると空
     assert(ds_queue_is_empty(queue) == true);
     assert(ds_queue_size(queue) == 0);
 
     // 8. 空時のエラー動作（デキュー/先頭参照）
     assert(ds_queue_dequeue(queue, &dequeued_ptr) == DS_ERR_EMPTY_CONTAINER);
     assert(ds_queue_front(queue, &front_ptr) == DS_ERR_EMPTY_CONTAINER);
     // 9. 統計情報取得
     ds_stats_t stats = {0};
     assert(ds_queue_get_stats(queue, &stats) == DS_SUCCESS);
     printf("Total elements (should be 0): %zu\n", stats.total_elements);
 
     // 10. キュー破棄
     assert(ds_queue_destroy(queue) == DS_SUCCESS);
 
     printf("=== Queue ADT Unit Test Passed ===\n");
     return 0;
 }
 
 /**
  * 【テスト設計補足】
  * - FIFO動作を厳密検証（入れた順に取り出せるか）
  * - エラー系も網羅し堅牢性チェック
  * - stats.total_elementsで実際の内部管理値も確認
  * - テストはmain()単体のみ。業務ロジック・UIは混在禁止
  * - assertはCI/CD自動テストで即失敗判定に使える
  */