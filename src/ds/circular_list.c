
#include "ds/circular_list.h"
#include <stdlib.h>

struct ds_circular_node {
    void* data;
    struct ds_circular_node* next;
};

struct ds_circular_list {
    struct ds_circular_node* tail;
    size_t size;
};

ds_error_t ds_circular_list_create(ds_circular_list_t** out_list) {
    if (!out_list) return DS_ERR_NULL_POINTER;
    ds_circular_list_t* list = malloc(sizeof(ds_circular_list_t));
    if (!list) return DS_ERR_ALLOC;
    list->tail = NULL;
    list->size = 0;
    *out_list = list;
    return DS_SUCCESS;
}

ds_error_t ds_circular_list_destroy(ds_circular_list_t* list) {
    if (!list) return DS_ERR_NULL_POINTER;
    if (list->tail) {
        struct ds_circular_node* cur = list->tail->next;
        struct ds_circular_node* stop = list->tail;
        while (cur != stop) {
            struct ds_circular_node* tmp = cur;
            cur = cur->next;
            free(tmp);
        }
        free(list->tail);
    }
    free(list);
    return DS_SUCCESS;
}

ds_error_t ds_circular_list_insert(ds_circular_list_t* list, void* data) {
    if (!list) return DS_ERR_NULL_POINTER;
    struct ds_circular_node* node = malloc(sizeof(struct ds_circular_node));
    if (!node) return DS_ERR_ALLOC;
    node->data = data;
    if (!list->tail) {
        node->next = node;
        list->tail = node;
    } else {
        node->next = list->tail->next;
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
    return DS_SUCCESS;
}

ds_error_t ds_circular_list_remove(ds_circular_list_t* list, void** out_data) {
    if (!list || !out_data) return DS_ERR_NULL_POINTER;
    if (!list->tail) return DS_ERR_EMPTY;
    struct ds_circular_node* head = list->tail->next;
    *out_data = head->data;
    if (list->tail == head) {
        free(head);
        list->tail = NULL;
    } else {
        list->tail->next = head->next;
        free(head);
    }
    list->size--;
    return DS_SUCCESS;
}

bool ds_circular_list_is_empty(const ds_circular_list_t* list) {
    return !list || !list->tail;
}

size_t ds_circular_list_size(const ds_circular_list_t* list) {
    return list ? list->size : 0;
}