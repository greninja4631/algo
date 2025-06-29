#include "../../include/data_structures.h"
#include "../../include/ds/lru_cache.h"
#include <stdlib.h>
#include <string.h>

// ノード構造体
typedef struct lru_node {
    int key;
    void* value;
    struct lru_node* prev;
    struct lru_node* next;
    struct lru_node* hnext;
} lru_node_t;

// 本体構造体
struct ds_lru_cache {
    size_t capacity;
    size_t size;
    lru_node_t* head;
    lru_node_t* tail;
    lru_node_t** map;
    size_t map_size;
};

// 内部ユーティリティ
static size_t hash(int key, size_t map_size) {
    return ((size_t)key) % map_size;
}

static void remove_node(ds_lru_cache_t* cache, lru_node_t* node) {
    if (node->prev) node->prev->next = node->next;
    else cache->head = node->next;
    if (node->next) node->next->prev = node->prev;
    else cache->tail = node->prev;
    node->prev = node->next = NULL;
}

static void insert_front(ds_lru_cache_t* cache, lru_node_t* node) {
    node->prev = NULL;
    node->next = cache->head;
    if (cache->head) cache->head->prev = node;
    cache->head = node;
    if (!cache->tail) cache->tail = node;
}

// --- 生成 ---
ds_lru_cache_t* ds_lru_cache_create(size_t capacity) {
    if (capacity == 0) {
        ds_log(DS_LOG_ERROR, "[lru_cache] キャパシティは1以上必要\n");
        return NULL;
    }
    ds_lru_cache_t* cache = (ds_lru_cache_t*)ds_malloc(sizeof(ds_lru_cache_t));
    if (!cache) {
        ds_log(DS_LOG_ERROR, "[lru_cache] 本体メモリ確保失敗\n");
        return NULL;
    }
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = cache->tail = NULL;
    cache->map_size = capacity * 4 + 7;
    cache->map = (lru_node_t**)ds_malloc(sizeof(lru_node_t*) * cache->map_size);
    if (!cache->map) {
        ds_log(DS_LOG_ERROR, "[lru_cache] map確保失敗\n");
        ds_free(cache);
        return NULL;
    }
    memset(cache->map, 0, sizeof(lru_node_t*) * cache->map_size);
    return cache;
}

// --- 破棄 ---
ds_error_t ds_lru_cache_destroy(ds_lru_cache_t* cache) {
    if (!cache) return DS_ERR_NULL_POINTER;
    lru_node_t* node = cache->head;
    while (node) {
        lru_node_t* next = node->next;
        if (node->value) ds_free(node->value); // 値もfreeする
        ds_free(node);
        node = next;
    }
    ds_free(cache->map);
    ds_free(cache);
    return DS_SUCCESS;
}

// --- 取得 ---
ds_error_t ds_lru_cache_get(ds_lru_cache_t* cache, int key, void** value) {
    if (!cache || !value) return DS_ERR_NULL_POINTER;
    size_t idx = hash(key, cache->map_size);
    lru_node_t* node = cache->map[idx];
    while (node) {
        if (node->key == key) {
            remove_node(cache, node);
            insert_front(cache, node);
            *value = node->value;
            return DS_SUCCESS;
        }
        node = node->hnext;
    }
    *value = NULL;
    ds_log(DS_LOG_INFO, "[lru_cache] キー%d見つからず\n", key);
    return DS_ERR_NOT_FOUND;
}

// --- 挿入/更新 ---
ds_error_t ds_lru_cache_put(ds_lru_cache_t* cache, int key, void* value) {
    if (!cache) return DS_ERR_NULL_POINTER;
    size_t idx = hash(key, cache->map_size);
    lru_node_t* node = cache->map[idx];

    // 既存keyなら値を更新して最前に移動
    while (node) {
        if (node->key == key) {
            if (node->value) ds_free(node->value);  // 以前の値をfree
            node->value = value;
            remove_node(cache, node);
            insert_front(cache, node);
            return DS_SUCCESS;
        }
        node = node->hnext;
    }

    // 容量超過時は末尾削除
    if (cache->size >= cache->capacity) {
        lru_node_t* del = cache->tail;
        if (del) {
            size_t del_idx = hash(del->key, cache->map_size);
            lru_node_t* p = cache->map[del_idx];
            lru_node_t* pprev = NULL;
            while (p) {
                if (p == del) {
                    if (pprev) pprev->hnext = p->hnext;
                    else cache->map[del_idx] = p->hnext;
                    break;
                }
                pprev = p;
                p = p->hnext;
            }
            remove_node(cache, del);
            if (del->value) ds_free(del->value);
            ds_free(del);
            cache->size--;
        }
    }

    // 新規ノード
    lru_node_t* newnode = (lru_node_t*)ds_malloc(sizeof(lru_node_t));
    if (!newnode) {
        ds_log(DS_LOG_ERROR, "[lru_cache] ノード確保失敗\n");
        return DS_ERR_OUT_OF_MEMORY;
    }
    newnode->key = key;
    newnode->value = value;
    newnode->prev = newnode->next = NULL;
    newnode->hnext = cache->map[idx];
    cache->map[idx] = newnode;
    insert_front(cache, newnode);
    cache->size++;
    return DS_SUCCESS;
}