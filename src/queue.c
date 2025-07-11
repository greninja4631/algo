// src/ds/queue.c

#include "ds/queue.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

// ---- 内部ノード型 ----
typedef struct ds_queue_node {
    void* data;
    struct ds_queue_node* next;
} ds_queue_node_t;

// ---- 不透明本体定義 ----
struct ds_queue {
    ds_queue_node_t* front;
    ds_queue_node_t* rear;
    size_t size;
};


// ---- API本体 ----

ds_error_t ds_queue_create(ds_queue_t** out_queue) {
    if (!out_queue) return DS_ERR_NULL_POINTER;
    ds_queue_t* q = (ds_queue_t*)malloc(sizeof(ds_queue_t));
    if (!q) return DS_ERR_ALLOC;
    q->front = q->rear = NULL;
    q->size = 0;
    *out_queue = q;
    return DS_SUCCESS;
}

ds_error_t ds_queue_destroy(ds_queue_t* queue) {
    if (!queue) return DS_ERR_NULL_POINTER;
    ds_queue_node_t* curr = queue->front;
    while (curr) {
        ds_queue_node_t* next = curr->next;
        free(curr);
        curr = next;
    }
    free(queue);
    return DS_SUCCESS;
}

ds_error_t ds_queue_enqueue(ds_queue_t* queue, void* data) {
    if (!queue) return DS_ERR_NULL_POINTER;
    ds_queue_node_t* node = (ds_queue_node_t*)malloc(sizeof(ds_queue_node_t));
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

ds_error_t ds_queue_dequeue(ds_queue_t* queue, void** out_data) {
    if (!queue || !out_data) return DS_ERR_NULL_POINTER;
    if (!queue->front) return DS_ERR_EMPTY;
    ds_queue_node_t* node = queue->front;
    *out_data = node->data;
    queue->front = node->next;
    if (!queue->front) queue->rear = NULL;
    free(node);
    queue->size--;
    return DS_SUCCESS;
}

ds_error_t ds_queue_front(const ds_queue_t* queue, void** out_data) {
    if (!queue || !out_data) return DS_ERR_NULL_POINTER;
    if (!queue->front) return DS_ERR_EMPTY;
    *out_data = queue->front->data;
    return DS_SUCCESS;
}

bool ds_queue_is_empty(const ds_queue_t* queue) {
    return (!queue || queue->size == 0);
}

size_t ds_queue_size(const ds_queue_t* queue) {
    return queue ? queue->size : 0;
}

// ---- stats: 本体のみでアクセス・他はforward宣言ポインタ扱い ----
ds_error_t ds_queue_get_stats(const ds_queue_t* queue, ds_stats_t* stats) {
    if (!queue || !stats) return DS_ERR_NULL_POINTER;
    stats->total_elements    = queue->size;
    stats->memory_allocated  = sizeof(ds_queue_t) + queue->size * sizeof(ds_queue_node_t);
    stats->operations_count  = 0; // 運用で適切に更新
    stats->creation_timestamp = 0; // 適宜拡張
    return DS_SUCCESS;
}