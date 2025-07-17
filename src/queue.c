#include "ds/queue.h"
#include "util/memory.h"
#include <string.h>

typedef struct ds_queue_node {
    void *data;
    struct ds_queue_node *next;
} node_t;

struct ds_queue {
    node_t *front;
    node_t *rear;
    size_t  size;
};

ds_error_t ds_queue_create(const ds_allocator_t *alloc, ds_queue_t **out_queue)
{
    if (!alloc || !out_queue) return DS_ERR_NULL_POINTER;
    ds_queue_t *q = ds_malloc(alloc, 1, sizeof *q);
    if (!q) return DS_ERR_ALLOC;
    memset(q, 0, sizeof *q);
    *out_queue = q;
    return DS_SUCCESS;
}

ds_error_t ds_queue_destroy(const ds_allocator_t *alloc, ds_queue_t *q)
{
    if (!alloc || !q) return DS_ERR_NULL_POINTER;
    node_t *n = q->front;
    while (n) {
        node_t *nx = n->next;
        ds_free(alloc, n);
        n = nx;
    }
    ds_free(alloc, q);
    return DS_SUCCESS;
}

ds_error_t ds_queue_enqueue(const ds_allocator_t *alloc, ds_queue_t *q, void *data)
{
    if (!alloc || !q) return DS_ERR_NULL_POINTER;
    node_t *n = ds_malloc(alloc, 1, sizeof *n);
    if (!n) return DS_ERR_ALLOC;
    n->data = data;
    n->next = NULL;
    if (!q->rear)
        q->front = q->rear = n;
    else {
        q->rear->next = n;
        q->rear = n;
    }
    q->size++;
    return DS_SUCCESS;
}

ds_error_t ds_queue_dequeue(const ds_allocator_t *alloc, ds_queue_t *q, void **out_data)
{
    if (!alloc || !q || !out_data) return DS_ERR_NULL_POINTER;
    if (!q->front) return DS_ERR_EMPTY;
    node_t *n = q->front;
    *out_data = n->data;
    q->front = n->next;
    if (!q->front) q->rear = NULL;
    ds_free(alloc, n);
    q->size--;
    return DS_SUCCESS;
}

ds_error_t ds_queue_front(const ds_queue_t *q, void **out_data)
{
    if (!q || !out_data) return DS_ERR_NULL_POINTER;
    if (!q->front) return DS_ERR_EMPTY;
    *out_data = q->front->data;
    return DS_SUCCESS;
}

bool ds_queue_is_empty(const ds_queue_t *q)
{
    return !q || q->size == 0;
}

size_t ds_queue_size(const ds_queue_t *q)
{
    return q ? q->size : 0;
}

ds_error_t ds_queue_get_stats(const ds_queue_t *q, ds_stats_t *stats)
{
    if (!q || !stats) return DS_ERR_NULL_POINTER;
    stats->total_elements = q->size;
    stats->memory_allocated = sizeof(ds_queue_t) + q->size * sizeof(node_t);
    stats->operations_count = 0;
    stats->creation_timestamp = 0;
    return DS_SUCCESS;
}
