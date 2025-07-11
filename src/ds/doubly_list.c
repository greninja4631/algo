// src/ds/doubly_list.c
#include "ds/doubly_list.h"    // APIヘッダ（ds_stats_t型もこれ経由でインクルードされる！）
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>              // time(NULL) を使う場合はここでだけ include

// --- 本体構造体・ノード構造体は.cでのみ定義 ---
struct ds_doubly_list_node {
    void* data;
    struct ds_doubly_list_node* prev;
    struct ds_doubly_list_node* next;
};

struct ds_doubly_list {
    struct ds_doubly_list_node* head;
    struct ds_doubly_list_node* tail;
    size_t size;
    ds_stats_t stats;   // ← stats型は data_structures.h 由来（再定義禁止！）
};

/* ユーティリティ関数やstatic関数もここでOK */

static struct ds_doubly_list_node* create_node(void* data) {
    struct ds_doubly_list_node* node = (struct ds_doubly_list_node*)malloc(sizeof(struct ds_doubly_list_node));
    if (!node) return NULL;
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

/* --- API実装 --- */

ds_doubly_list_t* ds_doubly_list_create(void) {
    ds_doubly_list_t* list = (ds_doubly_list_t*)malloc(sizeof(ds_doubly_list_t));
    if (!list) return NULL;
    list->head = list->tail = NULL;
    list->size = 0;
    memset(&list->stats, 0, sizeof(ds_stats_t));
    list->stats.creation_timestamp = (size_t)time(NULL); // タイムスタンプ
    return list;
}

ds_error_t ds_doubly_list_destroy(ds_doubly_list_t* list) {
    if (!list) return DS_ERR_NULL_POINTER;
    struct ds_doubly_list_node* node = list->head;
    while (node) {
        struct ds_doubly_list_node* next = node->next;
        free(node);
        node = next;
    }
    free(list);
    return DS_SUCCESS;
}

ds_error_t ds_doubly_list_insert_front(ds_doubly_list_t* list, void* data) {
    if (!list) return DS_ERR_NULL_POINTER;
    struct ds_doubly_list_node* node = create_node(data);
    if (!node) return DS_ERR_ALLOC;
    node->next = list->head;
    if (list->head) list->head->prev = node;
    list->head = node;
    if (!list->tail) list->tail = node;
    list->size++;
    list->stats.total_elements = list->size;
    list->stats.operations_count++;
    return DS_SUCCESS;
}

ds_error_t ds_doubly_list_insert_back(ds_doubly_list_t* list, void* data) {
    if (!list) return DS_ERR_NULL_POINTER;
    struct ds_doubly_list_node* node = create_node(data);
    if (!node) return DS_ERR_ALLOC;
    node->prev = list->tail;
    if (list->tail) list->tail->next = node;
    list->tail = node;
    if (!list->head) list->head = node;
    list->size++;
    list->stats.total_elements = list->size;
    list->stats.operations_count++;
    return DS_SUCCESS;
}

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
}

ds_error_t ds_doubly_list_remove_front(ds_doubly_list_t* list, void** out_data) {
    if (!list || !out_data) return DS_ERR_NULL_POINTER;
    if (!list->head) return DS_ERR_EMPTY;

    struct ds_doubly_list_node* node = list->head;
    *out_data = node->data;
    list->head = node->next;
    if (list->head) list->head->prev = NULL;
    else list->tail = NULL;

    free(node);
    list->size--;
    list->stats.total_elements = list->size;
    list->stats.operations_count++;
    return DS_SUCCESS;
}

ds_error_t ds_doubly_list_remove_back(ds_doubly_list_t* list, void** out_data) {
    if (!list || !out_data) return DS_ERR_NULL_POINTER;
    if (!list->tail) return DS_ERR_EMPTY;

    struct ds_doubly_list_node* node = list->tail;
    *out_data = node->data;
    list->tail = node->prev;
    if (list->tail) list->tail->next = NULL;
    else list->head = NULL;

    free(node);
    list->size--;
    list->stats.total_elements = list->size;
    list->stats.operations_count++;
    return DS_SUCCESS;
}

ds_error_t ds_doubly_list_remove_at(ds_doubly_list_t* list, size_t index, void** out_data) {
    if (!list || !out_data) return DS_ERR_NULL_POINTER;
    if (index >= list->size) return DS_ERR_OUT_OF_RANGE;
    if (index == 0) return ds_doubly_list_remove_front(list, out_data);
    if (index == list->size - 1) return ds_doubly_list_remove_back(list, out_data);

    struct ds_doubly_list_node* current = list->head;
    for (size_t i = 0; i < index; ++i) current = current->next;
    *out_data = current->data;

    current->prev->next = current->next;
    current->next->prev = current->prev;
    free(current);

    list->size--;
    list->stats.total_elements = list->size;
    list->stats.operations_count++;
    return DS_SUCCESS;
}

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