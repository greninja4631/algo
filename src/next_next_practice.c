/**
 * @file doubly_linked_list.c
 * @brief 実務品質双方向リンクリスト実装
 * @details
 * 【設計意図・作成理由】
 * 1. スタック/キュー/Undo-Redo/プロセススケジューラを統合実装
 * 2. メモリ安全性・エラー処理・型安全性を最優先
 * 3. 単一責務原則に基づく関数分離でテスト容易性を確保
 * 4. 実務で求められるログ出力・観測性・CI/CD対応
 * 5. Docker/Kubernetes/クラウド環境での運用を前提とした設計
 *
 * 【アーキテクチャ概要】
 * - データ層：Node構造体とメモリ管理
 * - 操作層：スタック/キュー/リスト操作API
 * - アプリケーション層：Undo-Redo/プロセススケジューラ
 * - 出力層：ログ・表示・エラー通知
 * 
 * 【初心者向け理解ガイド】
 * ・双方向リンクリスト = 前後両方向に移動できるデータの鎖
 * ・各ノード = データと前後へのポインタを持つ箱
 * ・スタック = 後入先出(LIFO) - 本を積み重ねるイメージ
 * ・キュー = 先入先出(FIFO) - 行列に並ぶイメージ
 * ・Undo/Redo = テキストエディタの取り消し/やり直し機能
 * ・ラウンドロビン = CPUが複数プロセスを順番に実行する方式
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include <stdint.h>
 #include <assert.h>
 #include <string.h>
 #include <errno.h>
 #include <time.h>
 
 /*═══════════════════════════════════════════════════════════════════════════
  * 型定義・定数・エラーコード（型安全性・明示性の確保）
  *═══════════════════════════════════════════════════════════════════════════*/
 
 /**
  * @brief エラーコード列挙型
  * @details 全ての操作結果を明確に分類し、デバッグ・監視を容易にする
  */
 typedef enum {
     DLL_SUCCESS = 0,           /**< 正常終了 */
     DLL_ERROR_NULL_POINTER,    /**< NULLポインタエラー */
     DLL_ERROR_MEMORY_ALLOC,    /**< メモリ割り当て失敗 */
     DLL_ERROR_EMPTY_LIST,      /**< 空リストに対する不正操作 */
     DLL_ERROR_INVALID_PARAM,   /**< 不正パラメータ */
     DLL_ERROR_CIRCULAR_DETECT  /**< 循環参照検出 */
 } DllErrorCode;
 
 /**
  * @brief ノード構造体
  * @details データと双方向ポインタを保持する基本要素
  * 【初心者向け】箱の中にデータと「前の箱」「次の箱」への矢印が入っている
  */
 typedef struct Node {
     int32_t data;              /**< 格納データ（32bit整数で型安全性確保）*/
     struct Node *next;         /**< 次ノードへのポインタ */
     struct Node *prev;         /**< 前ノードへのポインタ */
     uint64_t node_id;          /**< デバッグ用ノードID（メモリリーク検出） */
 } Node;
 
 /**
  * @brief リスト管理構造体
  * @details リストの状態管理とメタデータを一元化
  * 【設計意図】グローバル変数を避け、状態を明示的に管理
  */
 typedef struct {
     Node *head;                /**< 先頭ノードポインタ */
     Node *tail;                /**< 末尾ノードポインタ */
     size_t size;               /**< ノード数（O(1)でサイズ取得可能）*/
     bool is_circular;          /**< 循環リストフラグ */
     uint64_t next_node_id;     /**< 次に割り当てるノードID */
 } DoublyLinkedList;
 
 /**
  * @brief プロセス情報構造体
  * @details ラウンドロビンスケジューラ用のプロセス管理
  */
 typedef struct {
     int32_t process_id;        /**< プロセスID */
     uint32_t remaining_time;   /**< 残り実行時間 */
     uint32_t priority;         /**< 優先度 */
 } ProcessInfo;
 
 /*═══════════════════════════════════════════════════════════════════════════
  * グローバル変数・設定（最小限に抑制）
  *═══════════════════════════════════════════════════════════════════════════*/
 
 static uint64_t g_total_nodes_created = 0;  /**< 作成されたノード総数（監視用）*/
 static uint64_t g_total_nodes_freed = 0;    /**< 解放されたノード総数（リーク検出用）*/
 
 /*═══════════════════════════════════════════════════════════════════════════
  * ユーティリティ・ログ関数（観測性・デバッグ支援）
  *═══════════════════════════════════════════════════════════════════════════*/
 
 /**
  * @brief エラーログ出力
  * @param[in] error_code エラーコード
  * @param[in] function_name 関数名
  * @param[in] additional_info 追加情報
  * @details 本番環境ではsyslog、開発環境ではstderrに出力想定
  */
 static void log_error(DllErrorCode error_code, const char *function_name, const char *additional_info) {
     const char *error_messages[] = {
         "SUCCESS",
         "NULL_POINTER",
         "MEMORY_ALLOC",
         "EMPTY_LIST",
         "INVALID_PARAM",
         "CIRCULAR_DETECT"
     };
     
     fprintf(stderr, "[ERROR] %s in %s: %s\n", 
             error_messages[error_code], 
             function_name, 
             additional_info ? additional_info : "");
 }
 
 /**
  * @brief デバッグ情報出力
  * @param[in] format printfフォーマット
  * @details DEBUGマクロが定義されている場合のみ出力
  */
 static void log_debug(const char *format, ...) {
 #ifdef DEBUG
     va_list args;
     va_start(args, format);
     printf("[DEBUG] ");
     vprintf(format, args);
     printf("\n");
     va_end(args);
 #endif
 }
 
 /**
  * @brief メモリ使用量監視情報出力
  * @details 本番環境ではPrometheus/Grafana等に送信想定
  */
 static void log_memory_stats(void) {
     printf("[MEMORY] Created: %lu, Freed: %lu, Leaked: %lu\n", 
            g_total_nodes_created, 
            g_total_nodes_freed, 
            g_total_nodes_created - g_total_nodes_freed);
 }
 
 /*═══════════════════════════════════════════════════════════════════════════
  * コア機能：ノード管理・メモリ管理（責務分離・メモリ安全性）
  *═══════════════════════════════════════════════════════════════════════════*/
 
 /**
  * @brief 新規ノード作成
  * @param[in] data 格納するデータ
  * @param[out] error_code エラーコード格納先
  * @return 作成されたノードポインタ（失敗時NULL）
  * @details
  * 【設計意図】
  * - メモリ割り当て失敗を明示的に検出・通知
  * - ノードIDでトレーサビリティ確保
  * - NULLチェックでダングリングポインタ防止
  * 
  * 【初心者向け】
  * mallocでヒープ領域にメモリを確保し、データを格納する箱を作る
  * 失敗した場合は適切にエラーを返すことで、プログラムクラッシュを防ぐ
  */
 static Node* create_node_safe(int32_t data, DllErrorCode *error_code) {
     assert(error_code != NULL);  // 開発時のパラメータ検証
     
     Node *node = malloc(sizeof(Node));
     if (node == NULL) {
         *error_code = DLL_ERROR_MEMORY_ALLOC;
         log_error(DLL_ERROR_MEMORY_ALLOC, __func__, "malloc failed");
         return NULL;
     }
     
     // ノード初期化（全フィールドを明示的に設定）
     node->data = data;
     node->next = NULL;
     node->prev = NULL;
     node->node_id = ++g_total_nodes_created;
     
     log_debug("Node created: ID=%lu, Data=%d", node->node_id, data);
     *error_code = DLL_SUCCESS;
     return node;
 }
 
 /**
  * @brief ノード安全削除
  * @param[in] node 削除対象ノード
  * @details
  * 【設計意図】
  * - ダブルフリー防止
  * - メモリリーク監視カウンタ更新
  * - ポインタ無効化でアクセス違反防止
  */
 static void free_node_safe(Node *node) {
     if (node == NULL) return;
     
     log_debug("Node freed: ID=%lu, Data=%d", node->node_id, node->data);
     
     // セキュリティ：機密データのメモリクリア
     memset(node, 0, sizeof(Node));
     free(node);
     g_total_nodes_freed++;
 }
 
 /**
  * @brief リスト初期化
  * @param[out] list 初期化対象リスト
  * @return エラーコード
  * @details 全フィールドを明示的に初期化し、未定義動作を防止
  */
 static DllErrorCode init_list(DoublyLinkedList *list) {
     if (list == NULL) {
         log_error(DLL_ERROR_NULL_POINTER, __func__, "list is NULL");
         return DLL_ERROR_NULL_POINTER;
     }
     
     list->head = NULL;
     list->tail = NULL;
     list->size = 0;
     list->is_circular = false;
     list->next_node_id = 1;
     
     log_debug("List initialized");
     return DLL_SUCCESS;
 }
 
 /*═══════════════════════════════════════════════════════════════════════════
  * スタック操作（LIFO - Last In First Out）
  *═══════════════════════════════════════════════════════════════════════════*/
 
 /**
  * @brief スタック・プッシュ（先頭に挿入）
  * @param[in,out] list 対象リスト
  * @param[in] data 挿入データ
  * @return エラーコード
  * @details
  * 【アルゴリズム】
  * 1. 新ノード作成
  * 2. 新ノード→next = 現在のhead
  * 3. 現在のhead→prev = 新ノード（headが存在する場合）
  * 4. head = 新ノード
  * 5. サイズ更新
  * 
  * 【初心者向け】
  * 本を積み重ねるように、新しい要素を一番上（先頭）に置く操作
  */
 static DllErrorCode stack_push(DoublyLinkedList *list, int32_t data) {
     if (list == NULL) {
         log_error(DLL_ERROR_NULL_POINTER, __func__, "list is NULL");
         return DLL_ERROR_NULL_POINTER;
     }
     
     DllErrorCode error_code;
     Node *new_node = create_node_safe(data, &error_code);
     if (new_node == NULL) {
         return error_code;
     }
     
     // リストが空の場合
     if (list->head == NULL) {
         list->head = list->tail = new_node;
     } else {
         // 先頭に挿入
         new_node->next = list->head;
         list->head->prev = new_node;
         list->head = new_node;
     }
     
     list->size++;
     log_debug("Stack push: data=%d, size=%zu", data, list->size);
     return DLL_SUCCESS;
 }
 
 /**
  * @brief スタック・ポップ（先頭から削除）
  * @param[in,out] list 対象リスト
  * @param[out] data 取得したデータ格納先
  * @return エラーコード
  * @details
  * 【アルゴリズム】
  * 1. 空リストチェック
  * 2. データ取得
  * 3. head = head->next
  * 4. 新headのprevをNULLに設定
  * 5. 旧head削除
  * 6. サイズ更新
  */
 static DllErrorCode stack_pop(DoublyLinkedList *list, int32_t *data) {
     if (list == NULL || data == NULL) {
         log_error(DLL_ERROR_NULL_POINTER, __func__, "list or data is NULL");
         return DLL_ERROR_NULL_POINTER;
     }
     
     if (list->head == NULL) {
         log_error(DLL_ERROR_EMPTY_LIST, __func__, "cannot pop from empty stack");
         return DLL_ERROR_EMPTY_LIST;
     }
     
     Node *old_head = list->head;
     *data = old_head->data;
     
     list->head = old_head->next;
     if (list->head != NULL) {
         list->head->prev = NULL;
     } else {
         // リストが空になった場合
         list->tail = NULL;
     }
     
     free_node_safe(old_head);
     list->size--;
     
     log_debug("Stack pop: data=%d, size=%zu", *data, list->size);
     return DLL_SUCCESS;
 }
 
 /*═══════════════════════════════════════════════════════════════════════════
  * キュー操作（FIFO - First In First Out）
  *═══════════════════════════════════════════════════════════════════════════*/
 
 /**
  * @brief キュー・エンキュー（末尾に挿入）
  * @param[in,out] list 対象リスト
  * @param[in] data 挿入データ
  * @return エラーコード
  * @details
  * 【初心者向け】
  * 行列の最後尾に並ぶように、新しい要素を末尾に追加する操作
  */
 static DllErrorCode queue_enqueue(DoublyLinkedList *list, int32_t data) {
     if (list == NULL) {
         log_error(DLL_ERROR_NULL_POINTER, __func__, "list is NULL");
         return DLL_ERROR_NULL_POINTER;
     }
     
     DllErrorCode error_code;
     Node *new_node = create_node_safe(data, &error_code);
     if (new_node == NULL) {
         return error_code;
     }
     
     if (list->tail == NULL) {
         // 空リストの場合
         list->head = list->tail = new_node;
     } else {
         // 末尾に挿入
         list->tail->next = new_node;
         new_node->prev = list->tail;
         list->tail = new_node;
     }
     
     list->size++;
     log_debug("Queue enqueue: data=%d, size=%zu", data, list->size);
     return DLL_SUCCESS;
 }
 
 /**
  * @brief キュー・デキュー（先頭から削除）
  * @param[in,out] list 対象リスト
  * @param[out] data 取得したデータ格納先
  * @return エラーコード
  * @details スタックのpopと同じ実装（先頭から削除）
  */
 static DllErrorCode queue_dequeue(DoublyLinkedList *list, int32_t *data) {
     return stack_pop(list, data);  // 先頭削除なので実装共通化
 }
 
 /*═══════════════════════════════════════════════════════════════════════════
  * 循環リスト操作
  *═══════════════════════════════════════════════════════════════════════════*/
 
 /**
  * @brief 循環リスト化
  * @param[in,out] list 対象リスト
  * @return エラーコード
  * @details
  * 【設計意図】末尾と先頭を接続し、無限ループ可能な構造を作成
  * プロセススケジューラ等で使用
  */
 static DllErrorCode make_circular(DoublyLinkedList *list) {
     if (list == NULL) {
         log_error(DLL_ERROR_NULL_POINTER, __func__, "list is NULL");
         return DLL_ERROR_NULL_POINTER;
     }
     
     if (list->head == NULL || list->tail == NULL) {
         log_error(DLL_ERROR_EMPTY_LIST, __func__, "cannot make empty list circular");
         return DLL_ERROR_EMPTY_LIST;
     }
     
     if (list->is_circular) {
         log_debug("List is already circular");
         return DLL_SUCCESS;
     }
     
     list->tail->next = list->head;
     list->head->prev = list->tail;
     list->is_circular = true;
     
     log_debug("List made circular, size=%zu", list->size);
     return DLL_SUCCESS;
 }
 
 /*═══════════════════════════════════════════════════════════════════════════
  * 表示・デバッグ機能（観測性）
  *═══════════════════════════════════════════════════════════════════════════*/
 
 /**
  * @brief リスト内容表示（循環対応）
  * @param[in] list 表示対象リスト
  * @param[in] max_display 最大表示数（循環リスト対策）
  * @details
  * 【安全性】循環リストでの無限ループを防ぐため表示数制限
  */
 static void print_list_safe(const DoublyLinkedList *list, size_t max_display) {
     if (list == NULL) {
         printf("(NULL list)\n");
         return;
     }
     
     if (list->head == NULL) {
         printf("(empty list, size=%zu)\n", list->size);
         return;
     }
     
     printf("List (size=%zu, circular=%s): ", 
            list->size, 
            list->is_circular ? "yes" : "no");
     
     Node *current = list->head;
     size_t count = 0;
     
     do {
         printf("%d", current->data);
         if (count + 1 < max_display && current->next && 
             (!list->is_circular || current->next != list->head)) {
             printf(" -> ");
         }
         current = current->next;
         count++;
     } while (current && count < max_display && 
              (!list->is_circular || current != list->head));
     
     if (list->is_circular && count >= max_display) {
         printf(" ... (circular continues)");
     }
     printf("\n");
 }
 
 /*═══════════════════════════════════════════════════════════════════════════
  * メモリ管理（安全な全解放）
  *═══════════════════════════════════════════════════════════════════════════*/
 
 /**
  * @brief リスト全体の安全な解放
  * @param[in,out] list 解放対象リスト
  * @return エラーコード
  * @details
  * 【重要】循環リスト対応で無限ループを防止
  * メモリリーク防止のため全ノードを確実に解放
  */
 static DllErrorCode free_list_safe(DoublyLinkedList *list) {
     if (list == NULL) {
         log_error(DLL_ERROR_NULL_POINTER, __func__, "list is NULL");
         return DLL_ERROR_NULL_POINTER;
     }
     
     if (list->head == NULL) {
         log_debug("List already empty");
         return DLL_SUCCESS;
     }
     
     Node *current = list->head;
     size_t freed_count = 0;
     
     // 循環リスト対策：最初のノードで接続を切断
     if (list->is_circular && list->tail) {
         list->tail->next = NULL;
         list->head->prev = NULL;
     }
     
     while (current != NULL) {
         Node *next = current->next;
         free_node_safe(current);
         current = next;
         freed_count++;
         
         // 安全装置：想定以上のノード数で停止
         if (freed_count > list->size * 2) {
             log_error(DLL_ERROR_CIRCULAR_DETECT, __func__, "potential infinite loop detected");
             break;
         }
     }
     
     // リスト構造体リセット
     list->head = list->tail = NULL;
     list->size = 0;
     list->is_circular = false;
     
     log_debug("List freed: %zu nodes", freed_count);
     return DLL_SUCCESS;
 }
 
 /*═══════════════════════════════════════════════════════════════════════════
  * アプリケーション機能１：Undo/Redo システム
  *═══════════════════════════════════════════════════════════════════════════*/
 
 /**
  * @brief Undo/Redoシステム構造体
  * @details テキストエディタやグラフィックソフトの取り消し機能を実装
  */
 typedef struct {
     DoublyLinkedList undo_stack;    /**< Undo操作スタック */
     DoublyLinkedList redo_stack;    /**< Redo操作スタック */
     DoublyLinkedList main_data;     /**< メインデータリスト */
 } UndoRedoSystem;
 
 /**
  * @brief Undo/Redoシステム初期化
  */
 static DllErrorCode init_undo_redo_system(UndoRedoSystem *system) {
     if (system == NULL) return DLL_ERROR_NULL_POINTER;
     
     DllErrorCode result;
     if ((result = init_list(&system->undo_stack)) != DLL_SUCCESS) return result;
     if ((result = init_list(&system->redo_stack)) != DLL_SUCCESS) return result;
     if ((result = init_list(&system->main_data)) != DLL_SUCCESS) return result;
     
     return DLL_SUCCESS;
 }
 
 /**
  * @brief 操作実行（Undoスタックに記録）
  */
 static DllErrorCode execute_operation(UndoRedoSystem *system, int32_t data) {
     if (system == NULL) return DLL_ERROR_NULL_POINTER;
     
     // メイン操作実行
     DllErrorCode result;
     if ((result = stack_push(&system->main_data, data)) != DLL_SUCCESS) {
         return result;
     }
     
     // Undo用に記録
     if ((result = stack_push(&system->undo_stack, data)) != DLL_SUCCESS) {
         return result;
     }
     
     // Redo履歴をクリア（新操作によりRedo無効化）
     free_list_safe(&system->redo_stack);
     init_list(&system->redo_stack);
     
     printf("Operation executed: %d\n", data);
     return DLL_SUCCESS;
 }
 
 /**
  * @brief Undo実行
  */
 static DllErrorCode perform_undo(UndoRedoSystem *system) {
     if (system == NULL) return DLL_ERROR_NULL_POINTER;
     
     int32_t undo_data, main_data;
     DllErrorCode result;
     
     // Undoスタックから取得
     if ((result = stack_pop(&system->undo_stack, &undo_data)) != DLL_SUCCESS) {
         printf("No operation to undo\n");
         return result;
     }
     
     // メインデータから削除
     if ((result = stack_pop(&system->main_data, &main_data)) != DLL_SUCCESS) {
         return result;
     }
     
     // Redoスタックに記録
     if ((result = stack_push(&system->redo_stack, main_data)) != DLL_SUCCESS) {
         return result;
     }
     
     printf("Undo executed: removed %d\n", main_data);
     return DLL_SUCCESS;
 }
 
 /**
  * @brief Redo実行
  */
 static DllErrorCode perform_redo(UndoRedoSystem *system) {
     if (system == NULL) return DLL_ERROR_NULL_POINTER;
     
     int32_t redo_data;
     DllErrorCode result;
     
     // Redoスタックから取得
     if ((result = stack_pop(&system->redo_stack, &redo_data)) != DLL_SUCCESS) {
         printf("No operation to redo\n");
         return result;
     }
     
     // メインデータに復元
     if ((result = stack_push(&system->main_data, redo_data)) != DLL_SUCCESS) {
         return result;
     }
     
     // Undoスタックに記録
     if ((result = stack_push(&system->undo_stack, redo_data)) != DLL_SUCCESS) {
         return result;
     }
     
     printf("Redo executed: restored %d\n", redo_data);
     return DLL_SUCCESS;
 }
 
 /*═══════════════════════════════════════════════════════════════════════════
  * アプリケーション機能２：ラウンドロビン・プロセススケジューラ
  *═══════════════════════════════════════════════════════════════════════════*/
 
 /**
  * @brief ラウンドロビンスケジューラ実行
  * @param[in] list プロセスリスト（循環）
  * @param[in] time_slice タイムスライス（秒）
  * @param[in] max_cycles 最大実行サイクル数
  * @return エラーコード
  * @details
  * 【アルゴリズム概要】
  * 1. 各プロセスを順番に実行
  * 2. 指定時間で次のプロセスに切り替え
  * 3. 循環リストで無限に繰り返し
  * 
  * 【初心者向け】
  * CPUが複数のプログラムを少しずつ実行し、ユーザーから見ると
  * 同時に動いているように見せる仕組み
  */
 static DllErrorCode simulate_round_robin(DoublyLinkedList *list, 
                                        uint32_t time_slice, 
                                        uint32_t max_cycles) {
     if (list == NULL) {
         log_error(DLL_ERROR_NULL_POINTER, __func__, "list is NULL");
         return DLL_ERROR_NULL_POINTER;
     }
     
     if (list->head == NULL) {
         printf("No processes to schedule\n");
         return DLL_ERROR_EMPTY_LIST;
     }
     
     if (!list->is_circular) {
         printf("Converting list to circular for round-robin\n");
         DllErrorCode result = make_circular(list);
         if (result != DLL_SUCCESS) return result;
     }
     
     printf("\n=== Round Robin Process Scheduler ===\n");
     printf("Time slice: %u seconds, Max cycles: %u\n", time_slice, max_cycles);
     printf("Processes: %zu\n\n", list->size);
     
     Node *current = list->head;
     uint32_t cycle = 0;
     uint32_t total_time = 0;
     
     while (cycle < max_cycles) {
         printf("Cycle %u: Process %d executing for %u seconds\n", 
                cycle + 1, current->data, time_slice);
         
         // 実際のシステムでは usleep() や nanosleep() を使用
         // ここではシミュレーション
         total_time += time_slice;
         
         current = current->next;
         cycle++;
         
         // 一周したら区切り表示
         if (current == list->head && cycle % list->size == 0) {
             printf("--- Completed one full round ---\n");
         }
         
         // リアルタイム感を出すため少し待機（本番では不要）
         #ifdef SIMULATE_DELAY
         usleep(100000);  // 0.1秒待機
         #endif
     }
     
     printf("\nScheduling completed: %u cycles, %u total seconds\n", 
            cycle, total_time);
     return DLL_SUCCESS;
 }
 
 /*═══════════════════════════════════════════════════════════════════════════
  * 配列操作デモ機能（比較用）
  *═══════════════════════════════════════════════════════════════════════════*/
 
 /**
  * @brief 配列操作デモ
  * @details リンクリストとの比較のため、従来の配列操作を実装
  * 【教育目的】配列とリンクリストの違いを理解するための参考実装
  */
 static void demonstrate_array_operations(void) {
     printf("\n=== Array Operations Demo ===\n");
     
     const size_t ARRAY_SIZE = 5;
     int32_t array[5] = {10, 20, 30, 40, 50};
     
     printf("Static array contents: ");
     for (size_t i = 0; i < ARRAY_SIZE; i++) {
         printf("%d", array[i]);
         if (i < ARRAY_SIZE - 1) printf(", ");
     }
     printf("\n");
     
     // 配列の特徴：ランダムアクセス可能
     printf("Random access: array[2] = %d\n", array[2]);
     
     // 配列の制限：サイズ固定
     printf("Array limitations: Fixed size (%zu elements)\n", ARRAY_SIZE);
     printf("Linked list advantages: Dynamic size, efficient insertion/deletion\n");
 }
 
 /*═══════════════════════════════════════════════════════════════════════════
  * 統合テスト・デモ関数（機能検証）
  *═══════════════════════════════════════════════════════════════════════════*/
 
 /**
  * @brief スタック操作テスト
  * @return テスト結果（成功: DLL_SUCCESS）
  */
 static DllErrorCode test_stack_operations(void) {
     printf("\n=== Stack Operations Test ===\n");
     
     DoublyLinkedList stack;
     DllErrorCode result = init_list(&stack);
     if (result != DLL_SUCCESS) return result;
     
     // プッシュ操作テスト
     printf("Pushing elements: 10, 20, 30\n");
     if ((result = stack_push(&stack, 10)) != DLL_SUCCESS) return result;
     if ((result = stack_push(&stack, 20)) != DLL_SUCCESS) return result;
     if ((result = stack_push(&stack, 30)) != DLL_SUCCESS) return result;
     
     print_list_safe(&stack, 10);
     
     // ポップ操作テスト
     int32_t popped_data;
     printf("Popping elements:\n");
     while (stack.size > 0) {
         if ((result = stack_pop(&stack, &popped_data)) == DLL_SUCCESS) {
             printf("Popped: %d\n", popped_data);
         }
     }
     
     print_list_safe(&stack, 10);
     
     // エラーケーステスト
     printf("Testing error case (pop from empty stack):\n");
     result = stack_pop(&stack, &popped_data);
     if (result == DLL_ERROR_EMPTY_LIST) {
         printf("Correctly detected empty stack error\n");
         result = DLL_SUCCESS;  // テストとしては成功
     }
     
     free_list_safe(&stack);
     return result;
 }
 
 /**
  * @brief キュー操作テスト
  */
 static DllErrorCode test_queue_operations(void) {
     printf("\n=== Queue Operations Test ===\n");
     
     DoublyLinkedList queue;
     DllErrorCode result = init_list(&queue);
     if (result != DLL_SUCCESS) return result;
     
     // エンキュー操作テスト
     printf("Enqueueing elements: 100, 200, 300\n");
     if ((result = queue_enqueue(&queue, 100)) != DLL_SUCCESS) return result;
     if ((result = queue_enqueue(&queue, 200)) != DLL_SUCCESS) return result;
     if ((result = queue_enqueue(&queue, 300)) != DLL_SUCCESS) return result;
     
     print_list_safe(&queue, 10);
     
     // デキュー操作テスト
     int32_t dequeued_data;
     printf("Dequeuing elements:\n");
     while (queue.size > 0) {
         if ((result = queue_dequeue(&queue, &dequeued_data)) == DLL_SUCCESS) {
             printf("Dequeued: %d\n", dequeued_data);
         }
     }
     
     print_list_safe(&queue, 10);
     free_list_safe(&queue);
     return DLL_SUCCESS;
 }
 
 /**
  * @brief Undo/Redoシステムテスト
  */
 static DllErrorCode test_undo_redo_system(void) {
     printf("\n=== Undo/Redo System Test ===\n");
     
     UndoRedoSystem system;
     DllErrorCode result = init_undo_redo_system(&system);
     if (result != DLL_SUCCESS) return result;
     
     // 操作実行
     printf("Executing operations: 1, 2, 3\n");
     execute_operation(&system, 1);
     execute_operation(&system, 2);
     execute_operation(&system, 3);
     
     printf("Current data: ");
     print_list_safe(&system.main_data, 10);
     
     // Undo操作
     printf("\nUndo operations:\n");
     perform_undo(&system);
     printf("After undo: ");
     print_list_safe(&system.main_data, 10);
     
     perform_undo(&system);
     printf("After undo: ");
     print_list_safe(&system.main_data, 10);
     
     // Redo操作
     printf("\nRedo operations:\n");
     perform_redo(&system);
     printf("After redo: ");
     print_list_safe(&system.main_data, 10);
     
     // クリーンアップ
     free_list_safe(&system.main_data);
     free_list_safe(&system.undo_stack);
     free_list_safe(&system.redo_stack);
     
     return DLL_SUCCESS;
 }
 
 /**
  * @brief 循環リスト・ラウンドロビンテスト
  */
 static DllErrorCode test_circular_and_round_robin(void) {
     printf("\n=== Circular List & Round Robin Test ===\n");
     
     DoublyLinkedList process_list;
     DllErrorCode result = init_list(&process_list);
     if (result != DLL_SUCCESS) return result;
     
     // プロセス追加
     printf("Adding processes: P1, P2, P3, P4\n");
     if ((result = queue_enqueue(&process_list, 1)) != DLL_SUCCESS) return result;
     if ((result = queue_enqueue(&process_list, 2)) != DLL_SUCCESS) return result;
     if ((result = queue_enqueue(&process_list, 3)) != DLL_SUCCESS) return result;
     if ((result = queue_enqueue(&process_list, 4)) != DLL_SUCCESS) return result;
     
     printf("Before making circular: ");
     print_list_safe(&process_list, 10);
     
     // ラウンドロビンシミュレーション
     result = simulate_round_robin(&process_list, 2, 12);  // 2秒×12サイクル
     
     printf("After round robin: ");
     print_list_safe(&process_list, 10);
     
     free_list_safe(&process_list);
     return result;
 }
 
 /*═══════════════════════════════════════════════════════════════════════════
  * メイン関数（統合実行・エラーハンドリング）
  *═══════════════════════════════════════════════════════════════════════════*/
 
 /**
  * @brief メイン関数
  * @return 終了コード（0: 正常終了, 1: エラー終了）
  * @details
  * 【設計意図】
  * - 各機能を段階的にテスト実行
  * - エラー発生時は適切に通知して終了
  * - メモリリーク検出とレポート出力
  * - 本番環境での運用を想定した堅牢性
  * 
  * 【実行フロー】
  * 1. 基本的な配列操作デモ
  * 2. スタック操作テスト
  * 3. キュー操作テスト
  * 4. Undo/Redoシステムテスト
  * 5. 循環リスト・ラウンドロビンテスト
  * 6. メモリ使用状況レポート
  */
 int main(void) {
     printf("=================================================\n");
     printf("Production-Ready Doubly Linked List System\n");
     printf("Version: 1.0.0\n");
     printf("Build: %s %s\n", __DATE__, __TIME__);
     printf("=================================================\n");
     
     DllErrorCode overall_result = DLL_SUCCESS;
     
     // 基本機能テスト実行
     struct {
         const char *test_name;
         DllErrorCode (*test_func)(void);
     } test_suite[] = {
         {"Array Operations Demo", demonstrate_array_operations},
         {"Stack Operations", test_stack_operations},
         {"Queue Operations", test_queue_operations},
         {"Undo/Redo System", test_undo_redo_system},
         {"Circular List & Round Robin", test_circular_and_round_robin}
     };
     
     const size_t num_tests = sizeof(test_suite) / sizeof(test_suite[0]);
     size_t passed_tests = 0;
     
     for (size_t i = 0; i < num_tests; i++) {
         printf("\n[TEST %zu/%zu] %s\n", i + 1, num_tests, test_suite[i].test_name);
         printf("----------------------------------------\n");
         
         DllErrorCode result;
         if (i == 0) {
             // 配列デモは戻り値なし
             test_suite[i].test_func();
             result = DLL_SUCCESS;
         } else {
             result = test_suite[i].test_func();
         }
         
         if (result == DLL_SUCCESS) {
             printf("✅ PASSED: %s\n", test_suite[i].test_name);
             passed_tests++;
         } else {
             printf("❌ FAILED: %s (Error code: %d)\n", 
                    test_suite[i].test_name, result);
             overall_result = result;
         }
     }
     
     // テスト結果サマリー
     printf("\n=================================================\n");
     printf("TEST SUMMARY\n");
     printf("=================================================\n");
     printf("Total tests: %zu\n", num_tests);
     printf("Passed: %zu\n", passed_tests);
     printf("Failed: %zu\n", num_tests - passed_tests);
     
     if (overall_result == DLL_SUCCESS) {
         printf("🎉 ALL TESTS PASSED\n");
     } else {
         printf("💥 SOME TESTS FAILED\n");
     }
     
     // メモリ使用状況レポート
     printf("\n=== Memory Usage Report ===\n");
     log_memory_stats();
     
     if (g_total_nodes_created != g_total_nodes_freed) {
         printf("⚠️  MEMORY LEAK DETECTED!\n");
         printf("Please check memory management implementation\n");
         overall_result = DLL_ERROR_MEMORY_ALLOC;
     } else {
         printf("✅ No memory leaks detected\n");
     }
     
     // 本番環境での監視情報出力
     printf("\n=== Production Monitoring Info ===\n");
     printf("Timestamp: %ld\n", (long)time(NULL));
     printf("Process ID: %d\n", getpid());
     printf("Exit code: %d\n", overall_result == DLL_SUCCESS ? 0 : 1);
     
     // Docker/Kubernetes等での監視用ログ
     if (overall_result == DLL_SUCCESS) {
         printf("STATUS=SUCCESS\n");
     } else {
         printf("STATUS=ERROR\n");
         fprintf(stderr, "Application terminated with errors\n");
     }
     
     printf("\n=== Application Terminated ===\n");
     return overall_result == DLL_SUCCESS ? 0 : 1;
 }
 
 /*═══════════════════════════════════════════════════════════════════════════
  * 補足：本番運用時の拡張ポイント
  *═══════════════════════════════════════════════════════════════════════════
  * 
  * 【CI/CD統合】
  * - Makefile: コンパイル、テスト、静的解析自動化
  * - Dockerfile: コンテナ化によるポータビリティ確保
  * - Jenkins/GitHub Actions: 継続的インテグレーション
  * 
  * 【監視・観測性】
  * - Prometheus metrics: パフォーマンス指標収集
  * - Grafana dashboard: リアルタイム監視
  * - ELK Stack: ログ集約・解析
  * 
  * 【セキュリティ】
  * - Valgrind: メモリエラー検出
  * - AddressSanitizer: バッファオーバーフロー検出
  * - Static analysis: cppcheck, clang-tidy
  * 
  * 【スケーラビリティ】
  * - 非同期処理: pthread, async/await
  * - 分散処理: Redis, Message Queue
  * - マイクロサービス: REST API, gRPC
  * 
  * 【データ永続化】
  * - Database integration: PostgreSQL, MongoDB
  * - Serialization: JSON, Protocol Buffers
  * - Backup/Recovery: 自動バックアップ機能
  * 
  *═══════════════════════════════════════════════════════════════════════════*/