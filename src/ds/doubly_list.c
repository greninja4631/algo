/*======================================================================*
 *  include/ds/doubly_list.c
 *  ─────────────────────────
 *  – Opaque doubly-linked list, allocator-aware implementation
 *  – 実装はヘッダ公開せずにここで隠蔽（Guideline §1, §3）
 *======================================================================*/
#include "ds/doubly_list.h"   /* 外部 API 宣言     */
#include <stdlib.h>           /* size_t            */
#include <string.h>           /* memset            */
#include <time.h>             /* time()            */

/*─────────────────────  内部構造体  ─────────────────────*/
typedef struct ds_doubly_list_node {
    void                        *data;
    struct ds_doubly_list_node  *prev;
    struct ds_doubly_list_node  *next;
} ds_doubly_list_node_t;

struct ds_doubly_list {
    ds_doubly_list_node_t *head;
    ds_doubly_list_node_t *tail;
    size_t                 size;
    ds_stats_t             stats;      /* 拡張用統計情報 */
};

/*─────────────────  ユーティリティ forward  ─────────────*/
static ds_doubly_list_node_t *
new_node(const ds_allocator_t *alloc, void *data)
{
    ds_doubly_list_node_t *n = alloc->alloc(1, sizeof *n);
    if (!n) return NULL;
    n->data = data;
    n->prev = n->next = NULL;
    return n;
}

/*======================================================================*
 *  API IMPLEMENTATION
 *======================================================================*/
ds_error_t
ds_doubly_list_create(const ds_allocator_t *alloc,
                      ds_doubly_list_t    **out_list)
{
    if (!alloc || !out_list) return DS_ERR_NULL_POINTER;

    ds_doubly_list_t *l = alloc->alloc(1, sizeof *l);
    if (!l) return DS_ERR_ALLOC;

    memset(l, 0, sizeof *l);
    l->stats.creation_timestamp = (size_t)time(NULL);
    *out_list = l;
    return DS_SUCCESS;
}

ds_error_t
ds_doubly_list_destroy(const ds_allocator_t *alloc,
                       ds_doubly_list_t     *list)
{
    if (!alloc || !list) return DS_ERR_NULL_POINTER;

    ds_doubly_list_node_t *cur = list->head;
    while (cur) {
        ds_doubly_list_node_t *nxt = cur->next;
        alloc->free(cur);
        cur = nxt;
    }
    alloc->free(list);
    return DS_SUCCESS;
}

bool
ds_doubly_list_is_empty(const ds_doubly_list_t *list)
{
    return !list || list->size == 0;
}

size_t
ds_doubly_list_size(const ds_doubly_list_t *list)
{
    return list ? list->size : 0;
}

/*────────────  insert front/back/at  ────────────*/
static ds_error_t
insert_common(const ds_allocator_t *alloc,
              ds_doubly_list_t     *list,
              ds_doubly_list_node_t *pos,
              void                 *data,
              bool                  before)
{
    if (!alloc || !list) return DS_ERR_NULL_POINTER;

    ds_doubly_list_node_t *n = new_node(alloc, data);
    if (!n) return DS_ERR_ALLOC;

    if (!pos) {                /* empty list */
        list->head = list->tail = n;
    } else if (before) {       /* insert before pos */
        n->prev = pos->prev;
        n->next = pos;
        if (pos->prev) pos->prev->next = n;
        pos->prev ? (void)0 : (list->head = n);
        pos->prev = n;
    } else {                   /* insert after pos */
        n->next = pos->next;
        n->prev = pos;
        if (pos->next) pos->next->prev = n;
        pos->next ? (void)0 : (list->tail = n);
        pos->next = n;
    }
    list->size++;
    list->stats.total_elements = list->size;
    list->stats.operations_count++;
    return DS_SUCCESS;
}

ds_error_t
ds_doubly_list_insert_front(const ds_allocator_t *alloc,
                            ds_doubly_list_t     *list,
                            void                 *data)
{
    return insert_common(alloc, list, list ? list->head : NULL, data, true);
}

ds_error_t
ds_doubly_list_insert_back(const ds_allocator_t *alloc,
                           ds_doubly_list_t     *list,
                           void                 *data)
{
    return insert_common(alloc, list, list ? list->tail : NULL, data, false);
}

ds_error_t
ds_doubly_list_insert_at(const ds_allocator_t *alloc,
                         ds_doubly_list_t     *list,
                         size_t                idx,
                         void                 *data)
{
    if (!alloc || !list)            return DS_ERR_NULL_POINTER;
    if (idx > list->size)           return DS_ERR_OUT_OF_RANGE;
    if (idx == 0)                   return ds_doubly_list_insert_front(alloc, list, data);
    if (idx == list->size)          return ds_doubly_list_insert_back (alloc, list, data);

    ds_doubly_list_node_t *cur = list->head;
    for (size_t i = 0; i < idx; ++i) cur = cur->next;
    /* cur は idx 番目 */
    return insert_common(alloc, list, cur, data, true);
}

/*────────────  remove front/back/at  ────────────*/
static ds_error_t
remove_node(const ds_allocator_t *alloc,
            ds_doubly_list_t     *list,
            ds_doubly_list_node_t *node,
            void                **out)
{
    if (!alloc || !list || !node) return DS_ERR_NULL_POINTER;

    if (node->prev) node->prev->next = node->next;
    else            list->head       = node->next;
    if (node->next) node->next->prev = node->prev;
    else            list->tail       = node->prev;

    if (out) *out = node->data;
    alloc->free(node);
    list->size--;
    list->stats.total_elements = list->size;
    list->stats.operations_count++;
    return DS_SUCCESS;
}

ds_error_t
ds_doubly_list_remove_front(const ds_allocator_t *alloc,
                            ds_doubly_list_t     *list,
                            void                **out)
{
    if (!alloc || !list)        return DS_ERR_NULL_POINTER;
    if (list->size == 0)        return DS_ERR_EMPTY;
    return remove_node(alloc, list, list->head, out);
}

ds_error_t
ds_doubly_list_remove_back(const ds_allocator_t *alloc,
                           ds_doubly_list_t     *list,
                           void                **out)
{
    if (!alloc || !list)        return DS_ERR_NULL_POINTER;
    if (list->size == 0)        return DS_ERR_EMPTY;
    return remove_node(alloc, list, list->tail, out);
}

ds_error_t
ds_doubly_list_remove_at(const ds_allocator_t *alloc,
                         ds_doubly_list_t     *list,
                         size_t                idx,
                         void                **out)
{
    if (!alloc || !list)      return DS_ERR_NULL_POINTER;
    if (idx >= list->size)    return DS_ERR_OUT_OF_RANGE;

    ds_doubly_list_node_t *cur = list->head;
    for (size_t i = 0; i < idx; ++i) cur = cur->next;
    return remove_node(alloc, list, cur, out);
}

/*────────────  accessor  ────────────*/
ds_error_t
ds_doubly_list_get_at(const ds_doubly_list_t *list,
                      size_t                  idx,
                      void                  **out)
{
    if (!list || !out)      return DS_ERR_NULL_POINTER;
    if (idx >= list->size)  return DS_ERR_OUT_OF_RANGE;

    const ds_doubly_list_node_t *cur = list->head;
    for (size_t i = 0; i < idx; ++i) cur = cur->next;
    *out = cur->data;
    return DS_SUCCESS;
}

ds_error_t
ds_doubly_list_get_stats(const ds_doubly_list_t *list,
                         ds_stats_t             *out_stats)
{
    if (!list || !out_stats) return DS_ERR_NULL_POINTER;
    *out_stats = list->stats;
    return DS_SUCCESS;
}
