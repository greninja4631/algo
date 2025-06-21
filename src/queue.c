#include "data_structures.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

// 内部ノード型
typedef struct ds_queue_node {
    void* data;
    struct ds_queue_node* next;
} ds_queue_node_t;

// 不透明本体型
struct ds_queue {
    ds_queue_node_t* front;
    ds_queue_node_t* rear;
    size_t size;
};

// メモリ・ログ関数
static ds_malloc_func_t ds_malloc = malloc;
static ds_free_func_t ds_free = free;
static void default_log(ds_log_level_t level, const char* fmt, ...);
static ds_log_func_t ds_log = default_log;

// デフォルトログ関数
static void default_log(ds_log_level_t level, const char* fmt, ...) {
    (void)level;
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

/*-----------------------------------
 * API implementation
 *----------------------------------*/

// Queue Create
ds_queue_t* ds_queue_create(void) {
    if (!ds_malloc) ds_malloc = malloc;
    if (!ds_free) ds_free = free;
    if (!ds_log) ds_log = default_log;

    ds_queue_t* q = (ds_queue_t*)ds_malloc(sizeof(ds_queue_t));
    if (!q) {
        ds_log(DS_LOG_ERROR, "[ds_queue_create] Memory allocation failed\n");
        return NULL;
    }
    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

// Queue Destroy
ds_error_t ds_queue_destroy(ds_queue_t* queue) {
    if (!queue) return DS_ERR_NULL_POINTER;
    ds_queue_node_t* curr = queue->front;
    while (curr) {
        ds_queue_node_t* next = curr->next;
        if (ds_free) ds_free(curr);
        curr = next;
    }
    if (ds_free) ds_free(queue);
    return DS_SUCCESS;
}

// Enqueue
ds_error_t ds_queue_enqueue(ds_queue_t* queue, void* data) {
    if (!queue) return DS_ERR_NULL_POINTER;
    if (!ds_malloc) ds_malloc = malloc;
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

// Dequeue
ds_error_t ds_queue_dequeue(ds_queue_t* queue, void** data) {
    if (!queue || !data) return DS_ERR_NULL_POINTER;
    if (!queue->front) return DS_ERR_EMPTY;
    ds_queue_node_t* node = queue->front;
    *data = node->data;
    queue->front = node->next;
    if (!queue->front) queue->rear = NULL;
    if (ds_free) ds_free(node);
    queue->size--;
    return DS_SUCCESS;
}

// Front
ds_error_t ds_queue_front(const ds_queue_t* queue, void** data) {
    if (!queue || !data) return DS_ERR_NULL_POINTER;
    if (!queue->front) return DS_ERR_EMPTY;
    *data = queue->front->data;
    return DS_SUCCESS;
}

// Empty check
bool ds_queue_is_empty(const ds_queue_t* queue) {
    if (!queue) return true;
    return queue->size == 0;
}

// **Size getter (統一！)**
size_t ds_queue_size(const ds_queue_t* queue) {
    return queue ? queue->size : 0;
}

// Stats
ds_error_t ds_queue_get_stats(const ds_queue_t* queue, ds_stats_t* stats) {
    if (!queue || !stats) return DS_ERR_NULL_POINTER;
    stats->total_elements = queue->size;
    stats->memory_allocated = 0;
    stats->operations_count = 0;
    stats->creation_timestamp = 0;
    return DS_SUCCESS;
}

// Log function setter
void ds_set_log_function(ds_log_func_t func) {
    ds_log = func ? func : default_log;
}

// Memory function setter
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func) {
    ds_malloc = malloc_func ? malloc_func : malloc;
    ds_free = free_func ? free_func : free;
}