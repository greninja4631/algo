  /**
 * @file queue.c
 * @brief 汎用キューADT実装（シリコンバレー標準・責務分離・型安全・観測性/テスト容易性重視）
 * @version 1.0.0
 * @date 2025-05-28
 */

#include "data_structures.h"
#include <stdlib.h>
#include <stdio.h>

/* 内部ノード構造体（外部からは見えないようstatic定義） */
typedef struct ds_queue_node {
    void* data;
    struct ds_queue_node* next;
} ds_queue_node_t;

/* キュー本体（不透明ポインタ） */
struct ds_queue {
    ds_queue_node_t* front;
    ds_queue_node_t* rear;
    size_t size;
};

/* メモリ管理関数（依存性注入：テスト用/本番用差し替え可） */
static ds_malloc_func_t ds_malloc = malloc;
static ds_free_func_t ds_free = free;

/* ログ関数（依存性注入） */
static ds_log_func_t ds_log = NULL;
static void default_log(ds_log_level_t level, const char* fmt, ...) {
    (void)level; // ログレベル未使用
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

/* ------------------------------------
 * 公開API実装
 * ------------------------------------ */

/**
 * @brief キュー生成
 */
ds_queue_t* ds_queue_create(void) {
    ds_queue_t* q = (ds_queue_t*)ds_malloc(sizeof(ds_queue_t));
    if (!q) {
        if (ds_log) ds_log(DS_LOG_ERROR, "[ds_queue_create] メモリ確保失敗\n");
        return NULL;
    }
    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

/**
 * @brief キュー破棄
 */
ds_error_t ds_queue_destroy(ds_queue_t* queue) {
    if (!queue) return DS_ERR_NULL_POINTER;
    ds_queue_node_t* curr = queue->front;
    while (curr) {
        ds_queue_node_t* next = curr->next;
        ds_free(curr);
        curr = next;
    }
    ds_free(queue);
    return DS_SUCCESS;
}

/**
 * @brief キューにデータをエンキュー
 */

//  キューの「不変条件」について
// ルール1：空の場合

// front == NULL かつ rear == NULL
// 両方ともNULLでなければならない

// ルール2：要素が1つの場合

// front == rear かつ 両方とも同じノードを指す
// 前も後ろも同じ人

// ルール3：要素が2つ以上の場合

// front != NULL かつ rear != NULL
// 両方とも有効なノードを指す

//	不変条件：「どんなときも、キューの最後尾ノードの next はNULL！」である


//enqueue = キュー（queue）という「列」に新しいデータ（要素）を追加するための関数です。
 ds_error_t ds_queue_enqueue(ds_queue_t* queue, void* data) {
    if (!queue) return DS_ERR_NULL_POINTER;
    ds_queue_node_t* node = (ds_queue_node_t*)ds_malloc(sizeof(ds_queue_node_t));
    if (!node) return DS_ERR_ALLOC;
    node->data = data;
    node->next = NULL; 
    // 条件分岐の理由：キューが空の場合がif文の!queue->rearと、既にデータがある場合はelse ifで処理が異なるためです。
    if (!queue->rear) {
        queue->front = queue->rear = node;
    } else {
        queue->rear->next = node;//今の最後尾の人の後ろに並ばせる
        queue->rear = node;      //今の最後尾の人の後ろに並ぶことに成功したことをキュー本体の大元に情報を保存して更新する。
    }
    queue->size++;
    return DS_SUCCESS;
}

/**
 * @brief キューからデータをデキュー
 */
//【解説：queueとdequeの違い】
// 1. queue（キュー）
// 「front」：先頭。「取り出す側」（dequeueする場所）です。

// 「rear」：末尾。「追加する側」（enqueueする場所）です。

// 特徴：片方からしか取り出せない、片方からしか追加できない
// （例：行列の順番待ち、バスの乗車など）

// 2. deque（デック、両端キュー：double-ended queue）
// 「front」：先頭。「追加も削除もできる」場所です。

// 「back」または「rear」：末尾。「追加も削除もできる」場所です。

// 特徴：前からも後ろからもデータを追加・削除できる
// （例：両側にドアのあるバス、トランプの山札など）



ds_error_t ds_queue_dequeue(ds_queue_t* queue, void** data) {
    if (!queue || !data) return DS_ERR_NULL_POINTER;
    if (!queue->front) return DS_ERR_EMPTY;
    ds_queue_node_t* node = queue->front;
    //変数nodeはただの変数宣言だけであるため、構造体に記載があるかどうかは関係ない
    *data = node->data;
    queue->front = node->next;
    if (!queue->front) queue->rear = NULL;
    ds_free(node);
    queue->size--;
    return DS_SUCCESS;
}

/**
 * @brief キューの先頭データ取得（参照のみ）
 */
ds_error_t ds_queue_front(ds_queue_t* queue, void** data) {
    if (!queue || !data) return DS_ERR_NULL_POINTER;
    if (!queue->front) return DS_ERR_EMPTY; // ここで空ならエラーを返す

    *data = queue->front->data;   // ここまで来たら、queue->frontは絶対NULLではない
    return DS_SUCCESS;
}

/**
 * @brief キューが空か判定
 */
bool ds_queue_is_empty(const ds_queue_t* queue) {
    if (!queue) return true;
    return (queue->size == 0);
}

/**
 * @brief キューのサイズ取得
 */
int ds_queue_size(const ds_queue_t* queue, size_t* out_size) {
    if (!queue || !out_size) return DS_ERR_NULL_POINTER;

    *out_size = queue->size;
    return DS_SUCCESS;
}
/**
 * @brief ログ関数を差し替える（DevOps/テスト用）
 */
void ds_set_log_function(ds_log_func_t func) {
    ds_log = func ? func : default_log;
}

// 2. 意味・意図
// 	•	「ユーザーが自作のログ関数をセットしてきたら、それを使う」
// 	•	「何もセットされなかった（funcがNULL）なら、用意してあるdefault_logを使う」
// 	•	**「常に“どちらか必ず使える関数”がds_logにセットされる」ようにするための“安全装置”**です。


/**
 * @brief メモリ管理関数を差し替える（テスト・本番切り替え）
 */
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func) {
    ds_malloc = malloc_func ? malloc_func : malloc;
    ds_free = free_func ? free_func : free;
}