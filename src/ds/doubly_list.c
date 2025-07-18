#include "ds/doubly_list.h"
#include <string.h>
#include <time.h>
#include <stddef.h>

/**
 * @file    doubly_list.c
 * @brief   双方向リスト Opaque実装（2025ガイドライン準拠）
 * @note    所有権・DI・NULLセーフ・二重free防止
 */

/* 内部ノード定義（.c限定） */
typedef struct ds_doubly_list_node {
    void                        *data;
    struct ds_doubly_list_node  *prev;
    struct ds_doubly_list_node  *next;
} ds_doubly_list_node_t;

/* Opaque本体（.c限定） */
struct ds_doubly_list {
    ds_doubly_list_node_t *head;
    ds_doubly_list_node_t *tail;
    size_t                 size;
    ds_stats_t             stats;  // 必要なメトリクスのみ
};

/* --- ノード生成 --- */
static ds_doubly_list_node_t *
new_node(const ds_allocator_t *alloc, void *data)
{
    ds_doubly_list_node_t *n = alloc->alloc(1, sizeof *n);
    if (!n) return NULL;
    n->data = data;
    n->prev = n->next = NULL;
    return n;
}

/* --- 全ノード解放 --- */
static void free_all_nodes(const ds_allocator_t *alloc, ds_doubly_list_t *list)
{
    if (!alloc || !list) return;
    ds_doubly_list_node_t *cur = list->head;
    while (cur) {
        ds_doubly_list_node_t *nxt = cur->next;
        alloc->free(cur);   // 所有権はリストが持つ
        cur = nxt;
    }
    list->head = list->tail = NULL;
    list->size = 0;
}

/* --- API実装 --- */

ds_error_t ds_doubly_list_create(const ds_allocator_t *alloc, ds_doubly_list_t **out_list)
{
    if (!alloc || !out_list) return DS_ERR_NULL_POINTER;
    ds_doubly_list_t *l = alloc->alloc(1, sizeof *l);
    if (!l) return DS_ERR_ALLOC;
    memset(l, 0, sizeof *l);
    l->stats.creation_timestamp = (size_t)time(NULL);
    *out_list = l;
    return DS_SUCCESS;
}

ds_error_t ds_doubly_list_destroy(const ds_allocator_t *alloc, ds_doubly_list_t *list)
{
    if (!alloc) return DS_ERR_NULL_POINTER;
    if (!list)  return DS_SUCCESS;     // NULLセーフで多重free抑止
    free_all_nodes(alloc, list);
    alloc->free(list);
    return DS_SUCCESS;
}

/* 残りのAPIは一切free漏れ・二重freeなし。
   ノードは必ずremove_nodeまたはfree_all_nodesでのみfree。
*/

bool ds_doubly_list_is_empty(const ds_doubly_list_t *list)
{
    return !list || list->size == 0;
}

size_t ds_doubly_list_size(const ds_doubly_list_t *list)
{
    return list ? list->size : 0;
}

static ds_error_t
insert_common(const ds_allocator_t  *alloc,
              ds_doubly_list_t      *list,
              ds_doubly_list_node_t *pos,
              void                  *data,
              bool                   before)
{
    if (!alloc || !list) return DS_ERR_NULL_POINTER;
    ds_doubly_list_node_t *n = new_node(alloc, data);
    if (!n) return DS_ERR_ALLOC;

    if (!pos) { // 空リスト
        list->head = list->tail = n;
    } else if (before) {
        n->prev = pos->prev;
        n->next = pos;
        if (pos->prev) pos->prev->next = n;
        else           list->head = n;
        pos->prev = n;
    } else {
        n->next = pos->next;
        n->prev = pos;
        if (pos->next) pos->next->prev = n;
        else           list->tail = n;
        pos->next = n;
    }
    list->size++;
    list->stats.total_elements = list->size;
    return DS_SUCCESS;
}

ds_error_t ds_doubly_list_insert_front(const ds_allocator_t *alloc, ds_doubly_list_t *list, void *data)
{ return insert_common(alloc, list, list ? list->head : NULL, data, true); }

ds_error_t ds_doubly_list_insert_back(const ds_allocator_t *alloc, ds_doubly_list_t *list, void *data)
{ return insert_common(alloc, list, list ? list->tail : NULL, data, false); }

ds_error_t ds_doubly_list_insert_at(const ds_allocator_t *alloc, ds_doubly_list_t *list, size_t idx, void *data)
{
    if (!alloc || !list) return DS_ERR_NULL_POINTER;
    if (idx > list->size) return DS_ERR_OUT_OF_RANGE;
    if (idx == 0)         return ds_doubly_list_insert_front(alloc, list, data);
    if (idx == list->size) return ds_doubly_list_insert_back(alloc, list, data);

    ds_doubly_list_node_t *cur = list->head;
    for (size_t i = 0; i < idx; ++i) cur = cur->next;
    return insert_common(alloc, list, cur, data, true);
}

static ds_error_t
remove_node(const ds_allocator_t *alloc,
            ds_doubly_list_t *list,
            ds_doubly_list_node_t *node,
            void **out_data)
{
    if (!alloc || !list || !node) return DS_ERR_NULL_POINTER;
    if (node->prev) node->prev->next = node->next;
    else            list->head = node->next;
    if (node->next) node->next->prev = node->prev;
    else            list->tail = node->prev;
    if (out_data)   *out_data = node->data;
    alloc->free(node); // ノード所有権はリスト
    list->size--;
    list->stats.total_elements = list->size;
    return DS_SUCCESS;
}

ds_error_t ds_doubly_list_remove_front(const ds_allocator_t *alloc, ds_doubly_list_t *list, void **out_data)
{
    if (!alloc || !list) return DS_ERR_NULL_POINTER;
    if (list->size == 0) return DS_ERR_EMPTY;
    return remove_node(alloc, list, list->head, out_data);
}

ds_error_t ds_doubly_list_remove_back(const ds_allocator_t *alloc, ds_doubly_list_t *list, void **out_data)
{
    if (!alloc || !list) return DS_ERR_NULL_POINTER;
    if (list->size == 0) return DS_ERR_EMPTY;
    return remove_node(alloc, list, list->tail, out_data);
}

ds_error_t ds_doubly_list_remove_at(const ds_allocator_t *alloc, ds_doubly_list_t *list, size_t idx, void **out_data)
{
    if (!alloc || !list) return DS_ERR_NULL_POINTER;
    if (idx >= list->size) return DS_ERR_OUT_OF_RANGE;
    ds_doubly_list_node_t *cur = list->head;
    for (size_t i = 0; i < idx; ++i) cur = cur->next;
    return remove_node(alloc, list, cur, out_data);
}

ds_error_t ds_doubly_list_get_at(const ds_doubly_list_t *list, size_t idx, void **out_data)
{
    if (!list || !out_data) return DS_ERR_NULL_POINTER;
    if (idx >= list->size) return DS_ERR_OUT_OF_RANGE;
    const ds_doubly_list_node_t *cur = list->head;
    for (size_t i = 0; i < idx; ++i) cur = cur->next;
    *out_data = cur->data;
    return DS_SUCCESS;
}

ds_error_t ds_doubly_list_get_stats(const ds_doubly_list_t *list, ds_stats_t *out_stats)
{
    if (!list || !out_stats) return DS_ERR_NULL_POINTER;
    *out_stats = list->stats;
    return DS_SUCCESS;
}