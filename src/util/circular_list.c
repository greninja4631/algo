#include "data_structures.h"
#include <stdlib.h>

// リスト構造体定義
struct ds_circular_list {
    ds_circular_node_t* tail; // 最後尾ノード（tail->nextがhead）
    size_t size;
};

// 生成
ds_circular_list_t* ds_circular_list_create(void) {
    ds_circular_list_t* list = malloc(sizeof(ds_circular_list_t));
    if (!list) return NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

// 破棄
ds_error_t ds_circular_list_destroy(ds_circular_list_t* list) {
    if (!list) return DS_ERR_NULL_POINTER;
    if (list->tail) {
        ds_circular_node_t* cur = list->tail->next; // head
        ds_circular_node_t* stop = list->tail;
        while (cur != stop) {
            ds_circular_node_t* tmp = cur;
            cur = cur->next;
            free(tmp);
        }
        free(list->tail);
    }
    free(list);
    return DS_SUCCESS;
}

// 末尾追加（tailの直後＝headの前にもなる）
ds_error_t ds_circular_list_insert(ds_circular_list_t* list, void* data) {
    if (!list) return DS_ERR_NULL_POINTER;
    ds_circular_node_t* node = malloc(sizeof(ds_circular_node_t));
    if (!node) return DS_ERR_ALLOC;
    node->data = data;
    if (!list->tail) {
        // 最初の要素
        node->next = node; // 自分自身を指す
        list->tail = node;
    } else {
        node->next = list->tail->next; // head
        list->tail->next = node;
        list->tail = node; // tailを更新
    }
    list->size++;
    return DS_SUCCESS;
}

// 先頭削除
ds_error_t ds_circular_list_remove(ds_circular_list_t* list, void** data) {
    if (!list || !data) return DS_ERR_NULL_POINTER;
    if (!list->tail) return DS_ERR_EMPTY;
    ds_circular_node_t* head = list->tail->next;
    *data = head->data;
    if (list->tail == head) {
        // 1個だけ
        free(head);
        list->tail = NULL;
    } else {
        list->tail->next = head->next; // headを飛ばす
        free(head);
    }
    list->size--;
    return DS_SUCCESS;
}

// 空判定
bool ds_circular_list_is_empty(const ds_circular_list_t* list) {
    return !list || !list->tail;
}

// サイズ取得
size_t ds_circular_list_size(const ds_circular_list_t* list) {
    return list ? list->size : 0;
}
