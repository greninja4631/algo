#include <stddef.h>
/*======================================================================*
 *  src/ds/round_robin_scheduler.c
 *  Round-Robin Scheduler  –  2025-07 ガイドライン準拠
 *======================================================================*/
#include "ds/round_robin.h"
#include <stdlib.h>
#include <string.h>

/* ───── 内部ノード ───── */
typedef struct rr_node {
    void            *data;
    struct rr_node  *next;
} rr_node_t;

/* ───── Opaque 本体 ───── */
struct ds_round_robin_scheduler {
    rr_node_t *head;
    rr_node_t *tail;
    size_t     size;
    int32_t    quantum;  /* 今は未使用でも保持しておく */
};

/* ───── ユーティリティ ───── */
static void rr_clear(const ds_allocator_t *a, ds_round_robin_scheduler_t *s)
{
    if (!a || !s) return;
    for (rr_node_t *cur = s->head; cur; ) {
        rr_node_t *nxt = cur->next;
        a->free(cur);
        cur = nxt;
    }
    s->head = s->tail = NULL;
    s->size = 0;
}

/* ───── API ───── */
ds_error_t
ds_round_robin_scheduler_create(const ds_allocator_t *a,
                                int32_t               quantum,
                                ds_round_robin_scheduler_t **out)
{
    if (!a || !out) return DS_ERR_NULL_POINTER;
    ds_round_robin_scheduler_t *s = a->alloc(1, sizeof *s);
    if (!s) return DS_ERR_ALLOC;
    memset(s, 0, sizeof *s);
    s->quantum = quantum;
    *out = s;
    return DS_SUCCESS;
}

ds_error_t
ds_round_robin_scheduler_destroy(const ds_allocator_t *a,
                                 ds_round_robin_scheduler_t *s)
{
    if (!a || !s) return DS_ERR_NULL_POINTER;
    rr_clear(a, s);
    a->free(s);
    return DS_SUCCESS;
}

ds_error_t
ds_round_robin_scheduler_add_process(const ds_allocator_t *a,
                                     ds_round_robin_scheduler_t *s,
                                     const ds_process_t *proc)
{
    if (!a || !s || !proc) return DS_ERR_NULL_POINTER;
    rr_node_t *n = a->alloc(1, sizeof *n);
    if (!n) return DS_ERR_ALLOC;
    n->data = (void *)proc;   /* 所有権 transfer 済みとみなす */
    n->next = NULL;
    if (!s->tail) s->head = s->tail = n;
    else          s->tail = s->tail->next = n;
    ++s->size;
    return DS_SUCCESS;
}

ds_error_t
ds_round_robin_scheduler_get_next_process(const ds_allocator_t *a,
                                          ds_round_robin_scheduler_t *s,
                                          ds_process_t **out_proc)
{
    if (!a || !s || !out_proc) return DS_ERR_NULL_POINTER;
    if (!s->head) return DS_ERR_EMPTY;

    rr_node_t *n = s->head;
    *out_proc    = n->data;

    s->head = n->next;
    if (!s->head) s->tail = NULL;
    a->free(n);
    --s->size;
    return DS_SUCCESS;
}

/* helper */
size_t
ds_round_robin_scheduler_size(const ds_round_robin_scheduler_t *s)
{ return s ? s->size : 0; }