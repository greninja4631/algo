#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

// 内部ノード構造体
typedef struct ds_hashmap_node {
    char* key;
    void* value;
    struct ds_hashmap_node* next;
} ds_hashmap_node_t;

// ハッシュマップ本体
struct ds_hashmap {
    size_t capacity;
    size_t size;
    ds_hashmap_node_t** buckets;
    ds_hashmap_free_func_t key_free;
    ds_hashmap_free_func_t val_free;
};

// ハッシュ関数（FNV-1a）
static unsigned long _ds_hash_str(const char* str) {
    unsigned long hash = 2166136261ul;
    for (; *str; ++str)
        hash = (hash ^ (unsigned char)(*str)) * 16777619;
    return hash;
}

// --- API実装 ---
ds_error_t ds_hashmap_create(size_t capacity, ds_hashmap_free_func_t key_free, ds_hashmap_free_func_t val_free, ds_hashmap_t** out_map){
    if (!out_map) return DS_ERR_NULL_POINTER;
    ds_hashmap_t* map = calloc(1, sizeof(ds_hashmap_t));
    if (!map) return DS_ERR_ALLOC;
    map->capacity = capacity ? capacity : 16;
    map->buckets = calloc(map->capacity, sizeof(ds_hashmap_node_t*));
    if (!map->buckets) {
        free(map);
        return DS_ERR_ALLOC;
    }
    map->key_free = key_free;
    map->val_free = val_free;
    *out_map = map;
    return DS_SUCCESS;
}

ds_error_t ds_hashmap_destroy(ds_hashmap_t* map) {
    if (!map) return DS_ERR_NULL_POINTER;
    for (size_t i = 0; i < map->capacity; ++i) {
        ds_hashmap_node_t* node = map->buckets[i];
        while (node) {
            ds_hashmap_node_t* next = node->next;
            if (map->key_free) map->key_free(node->key);
            if (map->val_free) map->val_free(node->value);
            free(node);
            node = next;
        }
    }
    free(map->buckets);
    free(map);
    return DS_SUCCESS;
}

ds_error_t ds_hashmap_put(ds_hashmap_t* map, const char* key, void* value){
    if (!map || !key) return DS_ERR_NULL_POINTER;
    unsigned long hash = _ds_hash_str(key);
    size_t idx = hash % map->capacity;
    ds_hashmap_node_t* node = map->buckets[idx];
    while (node) {
        if (strcmp(node->key, key) == 0) return DS_ERR_INVALID_ARG; // Duplicate
        node = node->next;
    }
    node = malloc(sizeof(ds_hashmap_node_t));
    if (!node) return DS_ERR_ALLOC;
    node->key = strdup(key);
    node->value = value;
    node->next = map->buckets[idx];
    map->buckets[idx] = node;
    map->size++;
    return DS_SUCCESS;
}

ds_error_t ds_hashmap_get(ds_hashmap_t* map, const char* key, void** out_value) {
    if (!map || !key || !out_value) return DS_ERR_NULL_POINTER;
    unsigned long hash = _ds_hash_str(key);
    size_t idx = hash % map->capacity;
    ds_hashmap_node_t* node = map->buckets[idx];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            *out_value = node->value;
            return DS_SUCCESS;
        }
        node = node->next;
    }
    return DS_ERR_NOT_FOUND;
}

ds_error_t ds_hashmap_remove(ds_hashmap_t* map, const char* key) {
    if (!map || !key) return DS_ERR_NULL_POINTER;
    unsigned long hash = _ds_hash_str(key);
    size_t idx = hash % map->capacity;
    ds_hashmap_node_t* node = map->buckets[idx], *prev = NULL;
    while (node) {
        if (strcmp(node->key, key) == 0) {
            if (prev) prev->next = node->next;
            else map->buckets[idx] = node->next;
            if (map->key_free) map->key_free(node->key);
            if (map->val_free) map->val_free(node->value);
            free(node);
            map->size--;
            return DS_SUCCESS;
        }
        prev = node;
        node = node->next;
    }
    return DS_ERR_NOT_FOUND;
}

size_t ds_hashmap_size(const ds_hashmap_t* map) {
    return map ? map->size : 0;
}
