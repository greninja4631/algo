#include "logger.h"
<<<<<<< HEAD
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
=======
#include <stdlib.h>
#include "ds/lru_cache.h"
#include "ds/hashmap.h"
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
>>>>>>> feature
    node->next = cache->head;
    if (cache->head) cache->head->prev = node;
    cache->head = node;
    if (!cache->tail) cache->tail = node;
<<<<<<< HEAD
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
=======
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
>>>>>>> feature
}