#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

// --- 標準型・真偽値・整数幅など基礎Cライブラリ ---
#include <stddef.h>    // size_t, NULL, ポインタ演算など
#include <stdbool.h>   // bool, true, false を使う
#include <stdint.h>    // int32_t, uint64_tなど固定幅整数
#include "config.h"    // プロジェクト共通の設定や定数

#ifdef __cplusplus
extern "C" {
#endif

// --- エラーコード定義（APIの型安全な戻り値） ---
// エラー種類(enumで記載)は“型安全に返すため”
typedef enum {
    DS_SUCCESS = 0,              // 正常終了
    DS_ERR_NULL_POINTER,         // NULLポインタ参照
    DS_ERR_ALLOC,                // メモリ確保失敗
    DS_ERR_EMPTY,                // 空の構造体
    DS_ERR_INVALID_ARG,          // 不正な引数
    DS_ERR_OVERFLOW,             // オーバーフロー
    DS_ERR_UNDERFLOW,            // アンダーフロー
    DS_ERR_NOT_FOUND,            // 要素未発見
    DS_ERR_SYSTEM,               // システムエラー
    DS_ERR_OUT_OF_MEMORY,        // メモリ不足
    DS_ERR_EMPTY_CONTAINER,      // 空pop/dequeue等でのエラー
    DS_ERR_SYSTEM_FAILURE        // Undo/Redo等での致命的失敗
} ds_error_t;

// --- 既存の他プロジェクトやテスト用互換マクロ ---
// 以前から使われている他プロジェクト・テストコードとの互換性を保つため
// また、テストケースや外部呼び出し時に古い名称をそのまま使えるようにする

//下記は、そのエラー種類ごとに、「どういう意味か？」、「どう扱うか？」を設計・ドキュメント・運用ポリシーで定める。
#define DS_ERROR_NULL_POINTER      DS_ERR_NULL_POINTER     // NULLポインタ用のエイリアス（過去資産/他OSSと互換）
#define DS_ERROR_NOT_FOUND         DS_ERR_NOT_FOUND        // NOT FOUNDエラーのエイリアス（レガシーAPI互換）
#define DS_ERROR_EMPTY_CONTAINER   DS_ERR_EMPTY_CONTAINER  // 空データ構造エラーのエイリアス（テスト/既存コード対策）
#define DS_ERROR_INVALID_ARGUMENT  DS_ERR_INVALID_ARG      // 引数不正エラーのエイリアス（他API標準名に合わせる）

// --- RPN電卓（逆ポーランド記法計算機）の型とAPI ---
// typedefにより、structを書かずに"ds_rpn_calculator_t"だけで型指定できる
// 例: ds_rpn_calculator_t* calc; と書ける（毎回structを書く必要がない）
typedef struct ds_rpn_calculator ds_rpn_calculator_t;       // 電卓本体型
ds_rpn_calculator_t* ds_rpn_calculator_create(void);        // インスタンス生成
ds_error_t ds_rpn_calculator_destroy(ds_rpn_calculator_t*); // 破棄
ds_error_t ds_rpn_calculator_evaluate(ds_rpn_calculator_t*, const char* expr, double* result); // 式の評価
ds_error_t ds_rpn_calculator_reset(ds_rpn_calculator_t*);   // 内部状態リセット

const char* ds_error_string(ds_error_t error); // ds_error_t … エラーコード用の型（上記のenumでtypedef済み）
// error … 関数の引数（変数名）として使う // 「ds_error_t error」は「この型の変数errorを宣言している」という意味

// --- 統計データ型（全データ構造で利用可能な観測メトリクス） ---
typedef struct ds_stats {
    size_t total_elements;         // 登録要素数　今何個データが入ってる？（total_elements）
    size_t memory_allocated;       // ヒープ確保バイト数　どれくらいメモリ使ってる？（memory_allocated）
    size_t operations_count;       // API呼び出し回数　何回操作された？（operations_count）
    uint64_t creation_timestamp;   // 生成時刻(UNIX秒) いつ作られた？（creation_timestamp）
    // 必要に応じて項目追加OK
} ds_stats_t;

// --- Undo/Redo用コマンドパターン型 ---
typedef ds_error_t (*ds_command_fn_t)(void* data); // 関数ポインタ型

typedef struct {
    ds_command_fn_t execute;   // コマンド実行
    ds_command_fn_t undo;      // Undo実行
    void* data;                // 操作対象データ
} ds_command_t;

// --- Stack（スタック: LIFO）API宣言 ---
typedef struct ds_stack ds_stack_t;
ds_stack_t* ds_stack_create(void);                             // 生成
ds_error_t ds_stack_destroy(ds_stack_t*);                      // 破棄
ds_error_t ds_stack_push(ds_stack_t*, void* data);             // プッシュ
ds_error_t ds_stack_pop(ds_stack_t*, void** data);             // ポップ
ds_error_t ds_stack_peek(const ds_stack_t*, void** data);      // 先頭参照
bool ds_stack_is_empty(const ds_stack_t*);                     // 空判定
size_t ds_stack_size(const ds_stack_t*);                       // 要素数取得
ds_error_t ds_stack_get_stats(const ds_stack_t*, ds_stats_t*); // メトリクス取得
ds_error_t ds_stack_reset(ds_stack_t* stack);                  // ← ★ 追加



// --- Queue（キュー: FIFO）API宣言 ---
typedef struct ds_queue ds_queue_t;
ds_queue_t* ds_queue_create(void);                             // 生成
ds_error_t ds_queue_destroy(ds_queue_t*);                      // 破棄
ds_error_t ds_queue_enqueue(ds_queue_t*, void* data);          // エンキュー
ds_error_t ds_queue_dequeue(ds_queue_t*, void** data);         // デキュー
bool ds_queue_is_empty(const ds_queue_t*);                     // 空判定
size_t ds_queue_size(const ds_queue_t*);                       // 要素数取得
ds_error_t ds_queue_get_stats(const ds_queue_t*, ds_stats_t*); // メトリクス取得
ds_error_t ds_queue_front(const ds_queue_t*, void** data);     // 先頭参照

// --- Doubly List API（双方向リスト）---
// 構造体/関数プロトタイプは必要に応じて拡張

typedef struct ds_doubly_list ds_doubly_list_t;

// --- Undo/Redo履歴システムAPI ---
typedef struct ds_history_system ds_history_system_t;
ds_history_system_t* ds_history_system_create(size_t max_history); // 履歴システム生成
ds_error_t ds_history_system_destroy(ds_history_system_t*);        // 破棄
// ...追加API可

// --- OS/スケジューラAPI: ラウンドロビンなど ---
// プロセス構造体・スケジューラ本体・API

typedef struct ds_process {
    int process_id;         // プロセスID
    char name[32];          // プロセス名
    int priority;           // 優先度
    int cpu_time;           // 割り当てCPU時間
    int memory_size;        // 必要メモリ
    int remaining_time;     // 残り実行時間
    // 今後拡張可能
} ds_process_t;

typedef struct ds_round_robin_scheduler ds_round_robin_scheduler_t;
ds_round_robin_scheduler_t* ds_round_robin_scheduler_create(int time_quantum); // 生成
ds_error_t ds_round_robin_scheduler_destroy(ds_round_robin_scheduler_t*);      // 破棄
ds_error_t ds_round_robin_scheduler_add_process(ds_round_robin_scheduler_t*, const ds_process_t*); // 追加
ds_error_t ds_round_robin_scheduler_get_next_process(ds_round_robin_scheduler_t*, ds_process_t*);  // 次プロセス取得
ds_error_t ds_round_robin_scheduler_complete_process(ds_round_robin_scheduler_t*, int process_id); // 完了通知

// --- ログ機能: 任意のロガーに切り替え可能 ---
typedef enum {
    DS_LOG_DEBUG = 0, DS_LOG_INFO, DS_LOG_WARN, DS_LOG_ERROR, DS_LOG_FATAL
} ds_log_level_t;
typedef void (*ds_log_func_t)(ds_log_level_t level, const char* fmt, ...); // 可変長引数ロガー
void ds_set_log_function(ds_log_func_t func); // ロガー差し替え

// --- メモリ管理: malloc/free差し替え可能 ---
typedef void* (*ds_malloc_func_t)(size_t size);
typedef void  (*ds_free_func_t)(void* ptr);
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);

// --- テスト・CI用拡張ポイント ---
#ifdef DS_UNIT_TEST
// ユニットテスト専用APIなど
#endif

#ifdef __cplusplus
}
#endif
#endif /* DATA_STRUCTURES_H */


// typedef struct history_node {
//     ds_command_t command;
//     struct history_node* prev;
//     struct history_node* next;
// } history_node_t;

// // struct ds_history_system {
//     history_node_t* head;         // 最古の履歴 history_node_t*のポインターを使用して、history_node_tの構造体にアクセスすることができる
//     history_node_t* tail;         // 最新の履歴
//     history_node_t* current;      // 現在の位置（Undo/Redo判定用）
//     size_t size;                  // 履歴数
//     size_t max_history;           // 最大保持数（0なら無制限）
// };
