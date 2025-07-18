/*======================================================================*
 *  src/ds/circular_list.c
 *  Singly-linked circular list — Opaque型 + DI + ガイドライン完全遵守
 *======================================================================*/
#include "ds/circular_list.h"
#include <string.h>
#include <time.h>

/* ────────────────────── 内部ノード／本体 ──────────────────────*/
typedef struct cl_node {
    void           *data;
    struct cl_node *next;
} cl_node_t;

struct ds_circular_list {
    cl_node_t *tail;   /* tail->next がhead */
    size_t     size;
    ds_stats_t stats;
};

/* ────────────────────── 内部ユーティリティ ──────────────────────*/
static cl_node_t *
new_node(const ds_allocator_t *alloc, void *data)
{
    cl_node_t *n = alloc->alloc(1, sizeof *n);
    if (n) { n->data = data; n->next = NULL; }
    return n;
}

/* operations_count未使用警告防止マクロ */
#define DS_STATS_OPS_INC(list_)          \
    do {                                 \
        (list_)->stats.operations_count++;\
        (void)(list_)->stats.operations_count; \
    } while (0)

/*======================================================================*
 *  API 実装
 *======================================================================*/
ds_error_t
ds_circular_list_create(const ds_allocator_t *alloc,
                        ds_circular_list_t  **out_list)
{
    if (!alloc || !out_list) return DS_ERR_NULL_POINTER;
    ds_circular_list_t *l = alloc->alloc(1, sizeof *l);
    if (!l) return DS_ERR_ALLOC;
    memset(l, 0, sizeof *l);
    l->stats.creation_timestamp = (size_t)time(NULL);
    *out_list = l;
    return DS_SUCCESS;
}

ds_error_t
ds_circular_list_destroy(const ds_allocator_t *alloc,
                         ds_circular_list_t   *list)
{
    if (!alloc || !list) return DS_ERR_NULL_POINTER;

    if (list->tail) {
        cl_node_t *head = list->tail->next;
        cl_node_t *cur = head;
        do {
            cl_node_t *nxt = cur->next;
            alloc->free(cur);
            cur = nxt;
        } while (cur != head);
        list->tail = NULL;
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
    cl_node_t *n = new_node(alloc, data);
    if (!n) return DS_ERR_ALLOC;

    if (!list->tail) {              /* 初要素 */
        n->next   = n;
        list->tail = n;
    } else {                        /* tailの直後(head)に挿入・tailを更新 */
        n->next         = list->tail->next;
        list->tail->next = n;
        list->tail       = n;
    }
    list->size++;
    list->stats.total_elements = list->size;
    DS_STATS_OPS_INC(list);
    return DS_SUCCESS;
}

ds_error_t
ds_circular_list_remove(const ds_allocator_t *alloc,
                        ds_circular_list_t   *list,
                        void                **out_data)
{
    if (!alloc || !list || !out_data) return DS_ERR_NULL_POINTER;
    if (!list->tail) return DS_ERR_EMPTY;
    cl_node_t *head = list->tail->next;
    *out_data = head->data;
    if (head == list->tail) {    /* 1要素のみ */
        list->tail = NULL;
    } else {
        list->tail->next = head->next;
    }
    alloc->free(head);
    list->size--;
    list->stats.total_elements = list->size;
    DS_STATS_OPS_INC(list);
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