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
 typedef struct history_node {
     ds_command_t command;
     struct history_node* prev;
     struct history_node* next;
 } history_node_t;
 
 /**
  * @brief Undo/Redo履歴システムの本体
  */
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
 static void clone_command(ds_command_t* dest, const ds_command_t* src) {
     memcpy(dest, src, sizeof(ds_command_t));
     // 注意: dataメンバの内容がポインタの場合、必要に応じてコピー戦略を工夫する
 }
 
 /**
  * @brief 履歴ノード解放
  */
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
 ds_error_t ds_history_system_execute_command(ds_history_system_t* history, const ds_command_t* command) {
     if (!history || !command || !command->execute) return DS_ERR_NULL_POINTER;
 
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
 
     // コマンド実行
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
  * @brief 直前のコマンドをUndo
  */
 ds_error_t ds_history_system_undo(ds_history_system_t* history) {
     if (!history || !history->current) return DS_ERR_INVALID_ARG;
     if (!history->current->command.undo) return DS_ERR_SYSTEM_FAILURE;
     ds_error_t result = history->current->command.undo(history->current->command.data);
     if (result != DS_SUCCESS) return result;
     history->current = history->current->prev;
     return DS_SUCCESS;
 }
 
 /**
  * @brief UndoしたコマンドをRedo
  */
 ds_error_t ds_history_system_redo(ds_history_system_t* history) {
     if (!history || !history->current || !history->current->next)
         return DS_ERR_INVALID_ARG;
     history_node_t* next = history->current->next;
     if (!next->command.execute) return DS_ERR_SYSTEM_FAILURE;
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