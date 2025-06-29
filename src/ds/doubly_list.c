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
