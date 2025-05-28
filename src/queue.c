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
ds_error_t ds_queue_enqueue(ds_queue_t* queue, void* data) {
    if (!queue) return DS_ERR_NULL_POINTER;
    ds_queue_node_t* node = (ds_queue_node_t*)ds_malloc(sizeof(ds_queue_node_t));
    if (!node) return DS_ERR_ALLOC;
    node->data = data;
    node->next = NULL;
    if (!queue->rear) {
        queue->front = queue->rear = node;
    } else {
        queue->rear->next = node;
        queue->rear = node;
    }
    queue->size++;
    return DS_SUCCESS;
}

/**
 * @brief キューからデータをデキュー
 */
ds_error_t ds_queue_dequeue(ds_queue_t* queue, void** data) {
    if (!queue || !data) return DS_ERR_NULL_POINTER;
    if (!queue->front) return DS_ERR_EMPTY;
    ds_queue_node_t* node = queue->front;
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
    if (!queue->front) return DS_ERR_EMPTY;
    *data = queue->front->data;
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
size_t ds_queue_size(const ds_queue_t* queue) {
    if (!queue) return 0;
    return queue->size;
}

/**
 * @brief ログ関数を差し替える（DevOps/テスト用）
 */
void ds_set_log_function(ds_log_func_t func) {
    ds_log = func ? func : default_log;
}

/**
 * @brief メモリ管理関数を差し替える（テスト・本番切り替え）
 */
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func) {
    ds_malloc = malloc_func ? malloc_func : malloc;
    ds_free = free_func ? free_func : free;
}