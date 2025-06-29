#include "../../include/data_structures.h"
#include "../../include/ds/queue.h"
#include "ds/queue.h"
#include <stdlib.h>
#include <stdio.h>

// --- ノード構造体 ---
typedef struct ds_queue_node {
    void* data;
    struct ds_queue_node* next;
} ds_queue_node_t;

// --- キュー構造体本体 ---
struct ds_queue {
    ds_queue_node_t* head;
    ds_queue_node_t* tail;
    size_t size;
};

// --- キュー生成 ---
ds_queue_t* ds_queue_create(void) {
    ds_queue_t* q = (ds_queue_t*)ds_malloc(sizeof(ds_queue_t));
    if (!q) {
        ds_log(DS_LOG_ERROR, "[queue] メモリ確保失敗\n");
        return NULL;
    }
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

// --- キュー破棄 ---
ds_error_t ds_queue_destroy(ds_queue_t* q) {
    if (!q) return DS_ERR_NULL_POINTER;
    ds_queue_node_t* cur = q->head;
    while (cur) {
        ds_queue_node_t* tmp = cur;
        cur = cur->next;
        ds_free(tmp);
    }
    ds_free(q);
    return DS_SUCCESS;
}

// --- エンキュー（末尾追加）---
ds_error_t ds_queue_enqueue(ds_queue_t* q, void* data) {
    if (!q) return DS_ERR_NULL_POINTER;
    ds_queue_node_t* node = (ds_queue_node_t*)ds_malloc(sizeof(ds_queue_node_t));
    if (!node) {
        ds_log(DS_LOG_ERROR, "[queue] ノード確保失敗\n");
        return DS_ERR_ALLOC;
    }
    node->data = data;
    node->next = NULL;
    if (!q->tail) {
        q->head = q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
    q->size++;
    return DS_SUCCESS;
}

// --- デキュー（先頭取り出し）---
ds_error_t ds_queue_dequeue(ds_queue_t* q, void** data) {
    if (!q || !data) return DS_ERR_NULL_POINTER;
    if (!q->head) return DS_ERR_EMPTY_CONTAINER;
    ds_queue_node_t* node = q->head;
    *data = node->data;
    q->head = node->next;
    if (!q->head) q->tail = NULL;
    ds_free(node);
    q->size--;
    return DS_SUCCESS;
}

// --- 先頭要素を見る ---
ds_error_t ds_queue_front(const ds_queue_t* q, void** data) {
    if (!q || !data) return DS_ERR_NULL_POINTER;
    if (!q->head) return DS_ERR_EMPTY_CONTAINER;
    *data = q->head->data;
    return DS_SUCCESS;
}

// --- 空判定 ---
bool ds_queue_is_empty(const ds_queue_t* q) {
    return (!q || q->size == 0);
}

// --- サイズ取得 ---
size_t ds_queue_size(const ds_queue_t* q) {
    return q ? q->size : 0;
}

// --- 統計情報 ---
ds_error_t ds_queue_get_stats(const ds_queue_t* q, ds_stats_t* stats) {
    if (!q || !stats) return DS_ERR_NULL_POINTER;
    stats->total_elements = q->size;
    stats->memory_allocated = q->size * sizeof(ds_queue_node_t);
    stats->operations_count = 0; // 必要に応じて実装
    stats->creation_timestamp = 0;
    return DS_SUCCESS;
}
