/*======================================================================*
 *  src/ds/circular_list.c  ―  Allocator-aware singly-circular list
 *======================================================================*/
#include "ds/circular_list.h"
#include <string.h>     /* memset */
#include <time.h>       /* time   */

/* ───── 内部ノード & 本体 ───── */
typedef struct node {
    void        *data;
    struct node *next;
} node_t;

struct ds_circular_list {
    node_t    *tail;        /* tail->next が head（先頭）になる */
    size_t     size;
    ds_stats_t stats;
};

/* ───── ユーティリティ ───── */
static node_t *new_node(const ds_allocator_t *a, void *data)
{
    node_t *n = a->alloc(1, sizeof *n);
    if (n) { n->data = data; n->next = NULL; }
    return n;
}

/* ───── API 実装 ───── */
ds_error_t
ds_circular_list_create(const ds_allocator_t *alloc,
                        ds_circular_list_t  **out)
{
    if (!alloc || !out) return DS_ERR_NULL_POINTER;
    ds_circular_list_t *l = alloc->alloc(1, sizeof *l);
    if (!l) return DS_ERR_ALLOC;
    memset(l, 0, sizeof *l);
    l->stats.creation_timestamp = (size_t)time(NULL);
    *out = l;
    return DS_SUCCESS;
}

ds_error_t
ds_circular_list_destroy(const ds_allocator_t *alloc,
                         ds_circular_list_t   *list)
{
    if (!alloc || !list) return DS_ERR_NULL_POINTER;
    if (list->tail) {
        node_t *cur = list->tail->next;          /* head */
        do {
            node_t *nxt = cur->next;
            alloc->free(cur);
            cur = nxt;
        } while (cur != list->tail->next);
    }
    alloc->free(list);
    return DS_SUCCESS;
}

bool
ds_circular_list_is_empty(const ds_circular_list_t *list)
{
    return !list || list->size == 0;
}

size_t
ds_circular_list_size(const ds_circular_list_t *list)
{
    return list ? list->size : 0;
}

ds_error_t
ds_circular_list_insert(const ds_allocator_t *alloc,
                        ds_circular_list_t   *list,
                        void                 *data)
{
    if (!alloc || !list) return DS_ERR_NULL_POINTER;

    node_t *n = new_node(alloc, data);
    if (!n) return DS_ERR_ALLOC;

    if (!list->tail) {              /* 初要素 */
        n->next  = n;
        list->tail = n;
    } else {                        /* tail の直後(head)に挿入 */
        n->next        = list->tail->next;
        list->tail->next = n;
        list->tail      = n;        /* 新しい tail */
    }
    list->size++;
    list->stats.total_elements = list->size;
    list->stats.operations_count++;
    return DS_SUCCESS;
}

ds_error_t
ds_circular_list_remove(const ds_allocator_t *alloc,
                        ds_circular_list_t   *list,
                        void                **out)
{
    if (!alloc || !list || !out) return DS_ERR_NULL_POINTER;
    if (!list->tail)              return DS_ERR_EMPTY;

    node_t *head = list->tail->next;
    *out = head->data;

    if (head == list->tail) {       /* 要素が 1 つだけ */
        list->tail = NULL;
    } else {
        list->tail->next = head->next;  /* head を飛ばす */
    }
    alloc->free(head);
    list->size--;
    list->stats.total_elements = list->size;
    list->stats.operations_count++;
    return DS_SUCCESS;
}

ds_error_t
ds_circular_list_get_stats(const ds_circular_list_t *list,
                           ds_stats_t               *out_stats)
{
    if (!list || !out_stats) return DS_ERR_NULL_POINTER;
    *out_stats = list->stats;
    return DS_SUCCESS;
}
