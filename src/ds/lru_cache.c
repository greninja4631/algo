#include <stdlib.h>
#include "data_structures.h"
#include "ds/lru_cache.h"
#include "hashmap.h"
#include <string.h>


typedef struct ds_lru_node {
    char* key;
    void* value;
    struct ds_lru_node* prev;
    struct ds_lru_node* next;
} ds_lru_node_t;

struct ds_lru_cache {
    size_t capacity;
    size_t size;
    ds_lru_node_t* head; // 新しい順
    ds_lru_node_t* tail; // 古い順
    ds_hashmap_t* map;   // string→ds_lru_node_t*
};

// 内部関数
static void move_to_head(ds_lru_cache_t* cache, ds_lru_node_t* node);
static void remove_tail(ds_lru_cache_t* cache);


ds_error_t ds_lru_cache_create(size_t capacity, ds_lru_cache_t** out_cache) {
    if (!out_cache || capacity == 0) return DS_ERR_INVALID_ARG;
    ds_lru_cache_t* cache = calloc(1, sizeof(ds_lru_cache_t));
    if (!cache) return DS_ERR_ALLOC;
    cache->capacity = capacity;
    ds_error_t err = ds_hashmap_create(capacity, free, NULL, &cache->map);
    if (err != DS_SUCCESS) {
        free(cache);
        return err;
    }
    *out_cache = cache;
    return DS_SUCCESS;
}
ds_error_t ds_lru_cache_destroy(ds_lru_cache_t* cache) {
    if (!cache) return DS_ERR_NULL_POINTER;
    ds_lru_node_t* node = cache->head;
    while (node) {
        ds_lru_node_t* next = node->next;
        free(node->key);
        free(node);
        node = next;
    }
    ds_hashmap_destroy(cache->map);
    free(cache);
    return DS_SUCCESS;
}

ds_error_t ds_lru_cache_put(ds_lru_cache_t* cache, const char* key, void* value) {
    if (!cache || !key) return DS_ERR_NULL_POINTER;
    void* node_void = NULL;
    ds_error_t err = ds_hashmap_get(cache->map, key, &node_void);
    ds_lru_node_t* node = (err == DS_SUCCESS) ? (ds_lru_node_t*)node_void : NULL;

    if (node) {
        node->value = value;
        move_to_head(cache, node);
        return DS_SUCCESS;
    }

    node = calloc(1, sizeof(ds_lru_node_t));
    node->key = strdup(key);
    node->value = value;
    node->next = cache->head;
    if (cache->head) cache->head->prev = node;
    cache->head = node;
    if (!cache->tail) cache->tail = node;
    ds_hashmap_put(cache->map, node->key, node);
    cache->size++;

    if (cache->size > cache->capacity) remove_tail(cache);
    return DS_SUCCESS;
}

ds_error_t ds_lru_cache_get(ds_lru_cache_t* cache, const char* key, void** out_value) {
    if (!cache || !key || !out_value) return DS_ERR_NULL_POINTER;
    void* node_void = NULL;
    if (ds_hashmap_get(cache->map, key, &node_void) != DS_SUCCESS)
        return DS_ERR_NOT_FOUND;
    ds_lru_node_t* node = (ds_lru_node_t*)node_void;
    move_to_head(cache, node);
    *out_value = node->value;
    return DS_SUCCESS;
}

ds_error_t ds_lru_cache_size(const ds_lru_cache_t* cache, size_t* out_size) {
    if (!cache || !out_size) return DS_ERR_NULL_POINTER;
    *out_size = cache->size;
    return DS_SUCCESS;
}

// 内部関数：先頭移動
static void move_to_head(ds_lru_cache_t* cache, ds_lru_node_t* node) {
    if (node == cache->head) return;
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    if (cache->tail == node) cache->tail = node->prev;
    node->next = cache->head;
    node->prev = NULL;
    if (cache->head) cache->head->prev = node;
    cache->head = node;
}

// 内部関数：末尾削除
static void remove_tail(ds_lru_cache_t* cache) {
    ds_lru_node_t* tail = cache->tail;
    if (!tail) return;
    if (tail->prev) tail->prev->next = NULL;
    cache->tail = tail->prev;
    ds_hashmap_remove(cache->map, tail->key);
    free(tail->key);
    free(tail);
    cache->size--;
}