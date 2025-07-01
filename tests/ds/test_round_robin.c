/**
 * @file test_round_robin.c
 * @brief ラウンドロビンスケジューラ ADTユニットテスト
 * @version 1.0.0
 * @date 2025-05-28
 *
 * 【設計意図】
 * - スケジューラAPIの全動作パターン（正常・異常）を検証
 * - OS/クラウド現場品質を担保
 * - 拡張・リファクタ容易なテスト設計
 */

#include "data_structures.h"
 #include <stdio.h>
 #include <assert.h>
 #include <string.h>

 
 int main(void) {
     printf("=== Round Robin Scheduler ADT Unit Test Start ===\n");
 
     // 1. スケジューラ作成（タイムクォンタム10ms）
     ds_round_robin_scheduler_t* scheduler = ds_round_robin_scheduler_create(10);
     assert(scheduler != NULL);
 
     // 2. プロセス生成（擬似プロセス情報）
     ds_process_t p1 = {1, "proc-A", 5, 30, 1000};
     ds_process_t p2 = {2, "proc-B", 8, 20, 2000};
     ds_process_t p3 = {3, "proc-C", 3, 15, 3000};
 
     // 3. プロセス追加API
     assert(ds_round_robin_scheduler_add_process(scheduler, &p1) == DS_SUCCESS);
     assert(ds_round_robin_scheduler_add_process(scheduler, &p2) == DS_SUCCESS);
     assert(ds_round_robin_scheduler_add_process(scheduler, &p3) == DS_SUCCESS);
 
     // 4. 次プロセス取得（Round Robin 順番テスト）
     ds_process_t out;
     assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_SUCCESS);
     assert(out.process_id == 1); // p1
     assert(strcmp(out.name, "proc-A") == 0);
 
     assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_SUCCESS);
     assert(out.process_id == 2); // p2
 
     assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_SUCCESS);
     assert(out.process_id == 3); // p3
 
     // 再びp1に戻る（循環性）
     assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_SUCCESS);
     assert(out.process_id == 1);
 
     // 5. 実行完了通知API（p2を完了）
     assert(ds_round_robin_scheduler_complete_process(scheduler, 2) == DS_SUCCESS);
 
     // 6. 次プロセス取得、p2がスキップされることを確認
     assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_SUCCESS);
     assert(out.process_id != 2); // p2が消えている
 
     // 7. 異常系: NULLポインタ・未登録プロセス
     assert(ds_round_robin_scheduler_add_process(NULL, &p1) == DS_ERROR_NULL_POINTER);
     assert(ds_round_robin_scheduler_add_process(scheduler, NULL) == DS_ERROR_NULL_POINTER);
     assert(ds_round_robin_scheduler_complete_process(scheduler, 99) == DS_ERROR_NOT_FOUND);
     assert(ds_round_robin_scheduler_get_next_process(NULL, &out) == DS_ERROR_NULL_POINTER);
     assert(ds_round_robin_scheduler_get_next_process(scheduler, NULL) == DS_ERROR_NULL_POINTER);
 
     // 8. 残りプロセスを全て完了させる
     assert(ds_round_robin_scheduler_complete_process(scheduler, 1) == DS_SUCCESS);
     assert(ds_round_robin_scheduler_complete_process(scheduler, 3) == DS_SUCCESS);
 
     // 9. どのプロセスも無くなった時の挙動
     assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_ERROR_EMPTY_CONTAINER);
 
     // 10. 破棄
     assert(ds_round_robin_scheduler_destroy(scheduler) == DS_SUCCESS);
 
     printf("=== Round Robin Scheduler ADT Unit Test Passed ===\n");
     return 0;
 }
 
 /**
  * 【テスト設計補足】
  * - 正常系・異常系網羅（単体テスト/CI/CD自動化想定）
  * - 責務分離・テスト設計指針コメント付き
  * - main()はテスト用専用、実装は本体分離
  * - 拡張（新API追加）・テストカバレッジ容易化
  */