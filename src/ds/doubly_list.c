<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#include "logger.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "../../include/data_structures.h"
#include "../../include/ds/doubly_list.h"
#include "ds/doubly_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//----------------------
// ノード・本体定義
//----------------------
typedef struct ds_doubly_node {
    void* data;
    struct ds_doubly_node* prev;
    struct ds_doubly_node* next;
} ds_doubly_node_t;

struct ds_doubly_list {
    ds_doubly_node_t* head;
    ds_doubly_node_t* tail;
    size_t size;
};

//----------------------
// ユーティリティ
//----------------------
static ds_doubly_node_t* new_node(void* data) {
    ds_doubly_node_t* n = (ds_doubly_node_t*)ds_malloc(sizeof(ds_doubly_node_t));
    if (!n) {
        ds_log(DS_LOG_ERROR, "[doubly_list] ノード確保失敗\n");
        return NULL;
    }
    n->data = data;
    n->prev = n->next = NULL;
    return n;
}

//----------------------
// API本体
//----------------------

// 作成
ds_doubly_list_t* ds_doubly_list_create(void) {
    ds_doubly_list_t* list = (ds_doubly_list_t*)ds_malloc(sizeof(ds_doubly_list_t));
    if (!list) {
        ds_log(DS_LOG_ERROR, "[doubly_list] リスト本体確保失敗\n");
        return NULL;
    }
    list->head = list->tail = NULL;
    list->size = 0;
    return list;
}

// 破棄
ds_error_t ds_doubly_list_destroy(ds_doubly_list_t* list) {
    if (!list) return DS_ERR_NULL_POINTER;
    ds_doubly_node_t* cur = list->head;
    while (cur) {
        ds_doubly_node_t* next = cur->next;
        ds_free(cur);
        cur = next;
    }
    ds_free(list);
    return DS_SUCCESS;
}

// 先頭挿入
ds_error_t ds_doubly_list_insert_front(ds_doubly_list_t* list, void* data) {
    if (!list) return DS_ERR_NULL_POINTER;
    ds_doubly_node_t* n = new_node(data);
    if (!n) return DS_ERR_ALLOC;
    n->next = list->head;
    if (list->head) list->head->prev = n;
    list->head = n;
    if (!list->tail) list->tail = n;
    list->size++;
    return DS_SUCCESS;
}

// 末尾挿入
ds_error_t ds_doubly_list_insert_back(ds_doubly_list_t* list, void* data) {
    if (!list) return DS_ERR_NULL_POINTER;
    ds_doubly_node_t* n = new_node(data);
    if (!n) return DS_ERR_ALLOC;
    n->prev = list->tail;
    if (list->tail) list->tail->next = n;
    list->tail = n;
    if (!list->head) list->head = n;
    list->size++;
    return DS_SUCCESS;
}

// 指定位置挿入（0=front, size=back, それ以外はその直前）
ds_error_t ds_doubly_list_insert_at(ds_doubly_list_t* list, size_t idx, void* data) {
    if (!list) return DS_ERR_NULL_POINTER;
    if (idx > list->size) return DS_ERR_INVALID_ARG;
    if (idx == 0) return ds_doubly_list_insert_front(list, data);
    if (idx == list->size) return ds_doubly_list_insert_back(list, data);

    ds_doubly_node_t* n = new_node(data);
    if (!n) return DS_ERR_ALLOC;

    ds_doubly_node_t* cur = list->head;
    for (size_t i = 0; i < idx; i++) cur = cur->next;
    n->prev = cur->prev;
    n->next = cur;
    cur->prev->next = n;
    cur->prev = n;
    list->size++;
    return DS_SUCCESS;
}

// 先頭削除
ds_error_t ds_doubly_list_remove_front(ds_doubly_list_t* list, void** out_data) {
    if (!list || !out_data) return DS_ERR_NULL_POINTER;
    if (!list->head) return DS_ERR_EMPTY_CONTAINER;
    ds_doubly_node_t* n = list->head;
    *out_data = n->data;
    list->head = n->next;
    if (list->head) list->head->prev = NULL;
    else list->tail = NULL;
    ds_free(n);
    list->size--;
    return DS_SUCCESS;
}

// 末尾削除
ds_error_t ds_doubly_list_remove_back(ds_doubly_list_t* list, void** out_data) {
    if (!list || !out_data) return DS_ERR_NULL_POINTER;
    if (!list->tail) return DS_ERR_EMPTY_CONTAINER;
    ds_doubly_node_t* n = list->tail;
    *out_data = n->data;
    list->tail = n->prev;
    if (list->tail) list->tail->next = NULL;
    else list->head = NULL;
    ds_free(n);
    list->size--;
    return DS_SUCCESS;
}

// 指定位置削除
ds_error_t ds_doubly_list_remove_at(ds_doubly_list_t* list, size_t idx, void** out_data) {
    if (!list || !out_data) return DS_ERR_NULL_POINTER;
    if (idx >= list->size) return DS_ERR_INVALID_ARG;
    if (idx == 0) return ds_doubly_list_remove_front(list, out_data);
    if (idx == list->size - 1) return ds_doubly_list_remove_back(list, out_data);

    ds_doubly_node_t* cur = list->head;
    for (size_t i = 0; i < idx; i++) cur = cur->next;
    *out_data = cur->data;
    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;
    ds_free(cur);
    list->size--;
    return DS_SUCCESS;
}

// 指定位置参照
ds_error_t ds_doubly_list_get_at(const ds_doubly_list_t* list, size_t idx, void** out_data) {
    if (!list || !out_data) return DS_ERR_NULL_POINTER;
    if (idx >= list->size) return DS_ERR_INVALID_ARG;
    ds_doubly_node_t* cur = list->head;
    for (size_t i = 0; i < idx; i++) cur = cur->next;
    *out_data = cur->data;
    return DS_SUCCESS;
}

// 空判定
bool ds_doubly_list_is_empty(const ds_doubly_list_t* list) {
    return (!list || list->size == 0);
}

// サイズ取得
size_t ds_doubly_list_size(const ds_doubly_list_t* list) {
    return list ? list->size : 0;
}

// 統計情報
ds_error_t ds_doubly_list_get_stats(const ds_doubly_list_t* list, ds_stats_t* stats) {
    if (!list || !stats) return DS_ERR_NULL_POINTER;
    stats->total_elements = list->size;
    stats->memory_allocated = list->size * sizeof(ds_doubly_node_t);
    stats->operations_count = 0;
    stats->creation_timestamp = 0;
    return DS_SUCCESS;
}
=======
=======
>>>>>>> feature
#include <stdio.h>
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
=======
// src/ds/doubly_list.c
#include "ds/doubly_list.h"    // APIヘッダ（ds_stats_t型もこれ経由でインクルードされる！）
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>              // time(NULL) を使う場合はここでだけ include
>>>>>>> feature
=======
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
>>>>>>> feature
=======
#include "ds/doubly_list.h"
#include <string.h>
#include <time.h>
#include <stddef.h>
>>>>>>> feature

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

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#endif // DATA_STRUCTURES_H
>>>>>>> feature
=======
ds_error_t ds_doubly_list_insert_at(ds_doubly_list_t* list, size_t index, void* data) {
    if (!list) return DS_ERR_NULL_POINTER;
    if (index > list->size) return DS_ERR_OUT_OF_RANGE;
    if (index == 0) return ds_doubly_list_insert_front(list, data);
    if (index == list->size) return ds_doubly_list_insert_back(list, data);

    struct ds_doubly_list_node* current = list->head;
    for (size_t i = 0; i < index; ++i) current = current->next;
    struct ds_doubly_list_node* node = create_node(data);
    if (!node) return DS_ERR_ALLOC;

    node->prev = current->prev;
    node->next = current;
    current->prev->next = node;
    current->prev = node;

    list->size++;
    list->stats.total_elements = list->size;
    list->stats.operations_count++;
    return DS_SUCCESS;
=======
ds_error_t
ds_doubly_list_insert_back(const ds_allocator_t *alloc,
                           ds_doubly_list_t     *list,
                           void                 *data)
{
    return insert_common(alloc, list, list ? list->tail : NULL, data, false);
>>>>>>> feature
}
=======
ds_error_t ds_doubly_list_insert_back(const ds_allocator_t *alloc, ds_doubly_list_t *list, void *data)
{ return insert_common(alloc, list, list ? list->tail : NULL, data, false); }
>>>>>>> feature

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
<<<<<<< HEAD
}
<<<<<<< HEAD

ds_error_t ds_doubly_list_get_at(const ds_doubly_list_t* list, size_t index, void** out_data) {
    if (!list || !out_data) return DS_ERR_NULL_POINTER;
    if (index >= list->size) return DS_ERR_OUT_OF_RANGE;
    struct ds_doubly_list_node* current = list->head;
    for (size_t i = 0; i < index; ++i) current = current->next;
    *out_data = current->data;
    return DS_SUCCESS;
}

bool ds_doubly_list_is_empty(const ds_doubly_list_t* list) {
    if (!list) return true;
    return (list->size == 0);
}

size_t ds_doubly_list_size(const ds_doubly_list_t* list) {
    if (!list) return 0;
    return list->size;
}

ds_error_t ds_doubly_list_get_stats(const ds_doubly_list_t* list, ds_stats_t* stats) {
    if (!list || !stats) return DS_ERR_NULL_POINTER;
    *stats = list->stats;
    return DS_SUCCESS;
}
>>>>>>> feature
=======
>>>>>>> feature
=======
}
>>>>>>> feature
