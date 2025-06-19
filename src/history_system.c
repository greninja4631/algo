/**
 * @file history_system.c
 * @brief Undo/Redo履歴システム ADT 実装
 * @version 1.0.0
 * @date 2025-05-28
 *
 * 【設計方針】
 * - コマンドパターン+双方向リスト/スタックでUndo/Redoの履歴を管理
 * - ADTで外部から内部状態を隠蔽
 * - エラー処理・テスト容易性・依存性注入・情報隠蔽を徹底
 */

 #include "data_structures.h"
 #include <stdlib.h>
 #include <string.h>
 
 /* ---- 内部構造体定義 ---- */
 /**
  * @brief 履歴ノード（コマンド格納、前後の履歴を指す）
  */
  //「history_node」  履歴の「1件」や「1ノード」を表す構造体（データの最小単位）
 typedef struct history_node {
     ds_command_t command;
     struct history_node* prev; // ← 現在地からひとつ前のノード（過去の伝票）
     struct history_node* next; // ← 現在地からひとつ後ろのノード（未来の伝票）
 } history_node_t;
 
 /**
  * @brief Undo/Redo履歴システムの本体
  */

  //「ds_history_system」 履歴機能全体（ノードの集まり・履歴管理システム）をまとめて管理する構造体
 struct ds_history_system {
     history_node_t* head;         // 最古の履歴
     history_node_t* tail;         // 最新の履歴
     history_node_t* current;      // 現在の位置（Undo/Redo判定用）
     size_t size;                  // 履歴数
     size_t max_history;           // 最大保持数（0なら無制限）
 };
 
 /* ---- 内部ユーティリティ ---- */
 /**
  * @brief コマンドをディープコピー（コールバック関数・データごと）
  */

  //	•	clone_command関数は、「コマンドの内容をまるごと複製するための関数」
  //    Undo/Redoや履歴管理など「前の状態をそのまま再利用したい」場面でよく使われる
 static void clone_command(ds_command_t* dest, const ds_command_t* src) {
     memcpy(dest, src, sizeof(ds_command_t));
     // 注意: dataメンバの内容がポインタの場合、必要に応じてコピー戦略を工夫する
 }
 
 /**
  * @brief 履歴ノード解放
  */

  //free_node関数は「履歴ノード（history_node_t型）を使い終わった時に、きれいにメモリを開放するための関数」です。
  //Undo/Redo履歴のノードなど、途中で不要になった履歴や、最後に全部消すときに使います。
 static void free_node(history_node_t* node) {
     if (!node) return;
     // 必要に応じてnode->command.dataの解放も
     free(node);
 }
 
 /* ---- API実装 ---- */
 
 /**
  * @brief 履歴システム作成
  */
 ds_history_system_t* ds_history_system_create(size_t max_history) {
     ds_history_system_t* sys = (ds_history_system_t*)malloc(sizeof(ds_history_system_t));
     if (!sys) return NULL;
     sys->head = sys->tail = sys->current = NULL;
     sys->size = 0;
     sys->max_history = max_history;
     return sys;
 }
 
 /**
  * @brief 履歴システム破棄（全ノード/コマンドを解放）
  */
 ds_error_t ds_history_system_destroy(ds_history_system_t* history) {
     if (!history) return DS_ERR_NULL_POINTER;
     history_node_t* node = history->head;
     while (node) {
         history_node_t* next = node->next;
         // 必要なら command.data のメモリもfree
         free_node(node);
         node = next;
     }
     free(history);
     return DS_SUCCESS;
 }
 
 /**
  * @brief コマンドを実行し履歴に追加（Redo履歴は全て破棄、最大数超なら古いもの削除）
  */
 //新しいコマンド（操作）を実際に「適用」するための関数です。テキストを編集する・絵を描く・ファイルを保存する等、「何か操作」を反映させる
 ds_error_t ds_history_system_execute_command(ds_history_system_t* history, const ds_command_t* command) {
     if (!history || !command || !command->execute) return DS_ERR_NULL_POINTER;
 

//✅ マクドナルドの列（履歴リスト）に例える
// ● node = history->current->next;
// これは**「いま自分が並んでいる場所（current）」の“すぐ後ろにいる人”から処理を始める**という意味です。

// 例：「今、自分は5番目。5番目の人の後ろ（6番目の人）から順番にチェックする」

// ● history_node_t next = node->next;*
// これは**「今チェックしている人（node）の“さらに後ろの人”を覚えておく」**という意味です。

// 例えば「6番目の人を退席させる（削除する）前に、“その後ろに並んでいる7番目の人”をメモしておく」

     // Redo履歴を破棄
     history_node_t* node = history->current ? history->current->next : NULL;
     while (node) {
         history_node_t* next = node->next;
         free_node(node);
         node = next;
         history->size--;
     }
     if (history->current) history->current->next = NULL;
     history->tail = history->current;
 
     // 
     ds_error_t exec_result = command->execute(command->data);
     if (exec_result != DS_SUCCESS) return exec_result;
 
     // 新ノード作成
     history_node_t* new_node = (history_node_t*)malloc(sizeof(history_node_t));
     if (!new_node) return DS_ERR_OUT_OF_MEMORY;
     clone_command(&new_node->command, command);
     new_node->prev = history->current;
     new_node->next = NULL;
     if (history->current) history->current->next = new_node;
     history->current = new_node;

     //!history->head というif文の条件は「headがNULL（まだ履歴がひとつもない＝最初の追加）か？」を判定しています。 
     //もし履歴がまだ一つも無い場合（head==NULL）、その中で history->head = new_node; と「headに新しいノードをセット」できます。
     if (!history->head) history->head = new_node;
     history->tail = new_node;
     history->size++;
 
     // 最大履歴超過時は古いものから削除
     while (history->max_history > 0 && history->size > history->max_history) {
         history_node_t* old_head = history->head;
         history->head = old_head->next;
         if (history->head) history->head->prev = NULL;
         free_node(old_head);
         history->size--;
     }
 
     return DS_SUCCESS;
 }
 
 /**
  * @brief 
  * Undo = Ctrz+Zと同じ意味を持つ挙動のプログラム
  */
 ds_error_t ds_history_system_undo(ds_history_system_t* history) {
     if (!history || !history->current) return DS_ERR_INVALID_ARG;
     if (!history->current->command.undo) return DS_ERR_SYSTEM_FAILURE;
     ds_error_t result = history->current->command.undo(history->current->command.data);
     if (result != DS_SUCCESS) return result;
     //return resultで様々なエラーを再利用性・拡張性を保ちながら、出力できる
     history->current = history->current->prev;
     return DS_SUCCESS;
 }
 
 /**
  * @brief UndoしたコマンドをRedo
  */
 //Undoで「一歩下がった（ひとつ前に戻った）」状態から、 
 //Redoで「また一歩進み直す」＝“もとに戻した操作をもう一度やり直す”
 //ds_error_t を使用する理由は、execute関数を外部から呼び出しているため、エラー原因が不特定多数になりやすいため、対策してる。
 ds_error_t ds_history_system_redo(ds_history_system_t* history) {
     if (!history || !history->current || !history->current->next)
         return DS_ERR_INVALID_ARG;
     history_node_t* next = history->current->next;
     if (!next->command.execute) return DS_ERR_SYSTEM_FAILURE;

     //if (!next->command.execute) の意味は ,「次に実行すべきコマンド（nextノードのcommand）に、“実行用の関数（execute）がセットされていない”場合は、システムエラーとして返す」

     //99行目のexecute関数をexecute(next->command.data);で関数と引数を呼び出している。
     ds_error_t result = next->command.execute(next->command.data);
     if (result != DS_SUCCESS) return result;
     history->current = next;
     return DS_SUCCESS;
 }
 
 /**
  * @brief Undo可能かどうか判定
  */
 bool ds_history_system_can_undo(const ds_history_system_t* history) {
     return history && history->current && history->current->prev;
 }
 
 /**
  * @brief Redo可能かどうか判定
  */
 bool ds_history_system_can_redo(const ds_history_system_t* history) {
     return history && history->current && history->current->next;
 }
 
 /**
  * @brief 履歴を全てクリア
  */
 ds_error_t ds_history_system_clear(ds_history_system_t* history) {
     if (!history) return DS_ERR_NULL_POINTER;
     history_node_t* node = history->head;
     while (node) {
         history_node_t* next = node->next;
         free_node(node);
         node = next;
     }
     history->head = history->tail = history->current = NULL;
     history->size = 0;
     return DS_SUCCESS;
 }

 //ds_history_system_clear 「履歴（Undo/Redoノード）を全部クリア（＝中身だけ消して、システム本体は残す）」
 //ds_history_system_pop   「履歴の一番新しいノード（最新の1件）だけ削除」
 // ds_history_system_destroy 「履歴システム全体を破棄（ノードもhistory本体も全部free）」