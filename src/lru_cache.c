/**
 * @file lru_cache.c
 * @brief LRUキャッシュ実装（HashMap + Doubly Linked List合成）
 * - 最小・拡張容易設計・全操作O(1)設計
 */

#include "../include/data_structures.h"
#include "lru_cache.h"
#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

// ノード構造
typedef struct lru_node {
    char* key;
    void* value;
    struct lru_node* prev;
    struct lru_node* next;
} lru_node_t;

// 本体
struct lru_cache {
    size_t capacity;
    size_t size;
    lru_node_t* head; // 新しい順
    lru_node_t* tail; // 古い順
    hashmap_t* map;
};

// 内部関数（先頭・末尾のノード操作など）
// ...省略。テスト容易性重視の設計を徹底...

static void lru_move_to_head(lru_cache_t* cache, lru_node_t* node);
static void lru_remove_tail(lru_cache_t* cache);

lru_cache_t* lru_cache_create(size_t capacity) {
    if (capacity == 0) return NULL;
    lru_cache_t* cache = calloc(1, sizeof(lru_cache_t));
    cache->capacity = capacity;
    cache->map = hashmap_create(capacity, free, NULL); // keyはfree
    return cache;
}

void lru_cache_destroy(lru_cache_t* cache) {
    if (!cache) return;
    lru_node_t* node = cache->head;
    while (node) {
        lru_node_t* next = node->next;
        free(node->key);
        free(node);
        node = next;
    }
    hashmap_destroy(cache->map);
    free(cache);
}

hashmap_error_t lru_cache_put(lru_cache_t* cache, const char* key, void* value) {
    if (!cache || !key) return HM_ERR_NULL;
    void* node_void = NULL;
    hashmap_error_t err = hashmap_get(cache->map, key, &node_void);
    lru_node_t* node = (err == HM_OK) ? (lru_node_t*)node_void : NULL;

    if (node) {
        node->value = value;
        lru_move_to_head(cache, node);
        return HM_OK;
    }

    node = calloc(1, sizeof(lru_node_t));
    node->key = strdup(key);
    node->value = value;
    node->next = cache->head;
    if (cache->head) cache->head->prev = node;
    cache->head = node;
    if (!cache->tail) cache->tail = node;
    hashmap_put(cache->map, node->key, node);
    cache->size++;

    if (cache->size > cache->capacity) lru_remove_tail(cache);
    return HM_OK;
}

hashmap_error_t lru_cache_get(lru_cache_t* cache, const char* key, void** out_value) {
    if (!cache || !key || !out_value) return HM_ERR_NULL;
    void* node_void = NULL;
    if (hashmap_get(cache->map, key, &node_void) != HM_OK) return HM_ERR_KEY_NOT_FOUND;
    lru_node_t* node = (lru_node_t*)node_void;
    lru_move_to_head(cache, node);
    *out_value = node->value;
    return HM_OK;
}

static void lru_move_to_head(lru_cache_t* cache, lru_node_t* node) {
    if (node == cache->head) return;
    // ...ダブルリンクリスト操作で先頭へ...
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    if (cache->tail == node) cache->tail = node->prev;
    node->next = cache->head;
    node->prev = NULL;
    if (cache->head) cache->head->prev = node;
    cache->head = node;
}

static void lru_remove_tail(lru_cache_t* cache) {
    lru_node_t* tail = cache->tail;
    if (!tail) return;
    if (tail->prev) tail->prev->next = NULL;
    cache->tail = tail->prev;
    hashmap_remove(cache->map, tail->key);
    free(tail->key);
    free(tail);
    cache->size--;
}

size_t lru_cache_size(const lru_cache_t* cache) {
    return cache ? cache->size : 0;
}