/**
 * @file hashmap.c
 * @brief ハッシュテーブルの基本実装（チェイニング方式）
 * - 責務分離・エラー処理・静的変数
 * - CI/CD・テスト・クラウド対応しやすい実装
 */
#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

#define LOAD_FACTOR 0.75

/// チェイン用ノード
typedef struct hashmap_node {
    char* key;
    void* value;
    struct hashmap_node* next;
} hashmap_node_t;

/// HashMap本体
struct hashmap {
    size_t capacity;
    size_t size;
    hashmap_node_t** buckets;
    hm_free_func key_free;
    hm_free_func val_free;
};

// FNV-1a simple hash（改良可）
static unsigned long hash_str(const char* str) {
    unsigned long hash = 2166136261ul;
    for (; *str; ++str)
        hash = (hash ^ (unsigned char)(*str)) * 16777619;
    return hash;
}

hashmap_t* hashmap_create(size_t capacity, hm_free_func key_free, hm_free_func val_free) {
    hashmap_t* map = calloc(1, sizeof(hashmap_t));
    if (!map) return NULL;
    map->capacity = capacity ? capacity : 16;
    map->buckets = calloc(map->capacity, sizeof(hashmap_node_t*));
    map->key_free = key_free;
    map->val_free = val_free;
    return map;
}

void hashmap_destroy(hashmap_t* map) {
    if (!map) return;
    for (size_t i = 0; i < map->capacity; ++i) {
        hashmap_node_t* node = map->buckets[i];
        while (node) {
            hashmap_node_t* next = node->next;
            if (map->key_free) map->key_free(node->key);
            if (map->val_free) map->val_free(node->value);
            free(node);
            node = next;
        }
    }
    free(map->buckets);
    free(map);
}

hashmap_error_t hashmap_put(hashmap_t* map, const char* key, void* value) {
    if (!map || !key) return HM_ERR_NULL;
    unsigned long hash = hash_str(key);
    size_t idx = hash % map->capacity;
    hashmap_node_t* node = map->buckets[idx];
    while (node) {
        if (strcmp(node->key, key) == 0) return HM_ERR_DUPLICATE_KEY;
        node = node->next;
    }
    node = malloc(sizeof(hashmap_node_t));
    if (!node) return HM_ERR_ALLOC;
    node->key = strdup(key);
    node->value = value;
    node->next = map->buckets[idx];
    map->buckets[idx] = node;
    map->size++;
    return HM_OK;
}

hashmap_error_t hashmap_get(hashmap_t* map, const char* key, void** out_value) {
    if (!map || !key || !out_value) return HM_ERR_NULL;
    unsigned long hash = hash_str(key);
    size_t idx = hash % map->capacity;
    hashmap_node_t* node = map->buckets[idx];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            *out_value = node->value;
            return HM_OK;
        }
        node = node->next;
    }
    return HM_ERR_KEY_NOT_FOUND;
}

hashmap_error_t hashmap_remove(hashmap_t* map, const char* key) {
    if (!map || !key) return HM_ERR_NULL;
    unsigned long hash = hash_str(key);
    size_t idx = hash % map->capacity;
    hashmap_node_t* node = map->buckets[idx], *prev = NULL;
    while (node) {
        if (strcmp(node->key, key) == 0) {
            if (prev) prev->next = node->next;
            else map->buckets[idx] = node->next;
            if (map->key_free) map->key_free(node->key);
            if (map->val_free) map->val_free(node->value);
            free(node);
            map->size--;
            return HM_OK;
        }
        prev = node;
        node = node->next;
    }
    return HM_ERR_KEY_NOT_FOUND;
}

size_t hashmap_size(const hashmap_t* map) {
    return map ? map->size : 0;
}