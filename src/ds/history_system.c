#include "../../include/data_structures.h"
#include "../../include/hashmap.h"
#include <stdlib.h>
#include <string.h>

// --- 定数 ---
#define DS_HASHMAP_INIT_BUCKETS 64

// --- 内部エントリ型 ---
typedef struct ds_hashmap_entry {
    char* key;
    void* value;
    struct ds_hashmap_entry* next;
} ds_hashmap_entry_t;

// --- 本体構造体 ---
struct ds_hashmap {
    ds_hashmap_entry_t** buckets;
    size_t num_buckets;
    size_t size;
    ds_hashmap_free_func key_free;
    ds_hashmap_free_func val_free;
};

// --- 内部: ハッシュ関数 ---
static unsigned long ds_hashmap_hash(const char* str) {
    unsigned long h = 5381;
    int c;
    while ((c = *str++))
        h = ((h << 5) + h) + c;
    return h;
}

// --- 内部: 新規エントリ作成 ---
static ds_hashmap_entry_t* ds_hashmap_new_entry(const char* key, void* value) {
    ds_hashmap_entry_t* e = (ds_hashmap_entry_t*)ds_malloc(sizeof(ds_hashmap_entry_t));
    if (!e) {
        ds_log(DS_LOG_ERROR, "[ds_hashmap] エントリ確保失敗\n");
        return NULL;
    }
    e->key = strdup(key);
    if (!e->key) {
        ds_free(e);
        ds_log(DS_LOG_ERROR, "[ds_hashmap] キー確保失敗\n");
        return NULL;
    }
    e->value = value;
    e->next = NULL;
    return e;
}

// --- API実装 ---

// 作成
ds_hashmap_t* ds_hashmap_create(size_t capacity, ds_hashmap_free_func key_free, ds_hashmap_free_func val_free) {
    ds_hashmap_t* map = (ds_hashmap_t*)ds_malloc(sizeof(ds_hashmap_t));
    if (!map) {
        ds_log(DS_LOG_ERROR, "[ds_hashmap] 本体確保失敗\n");
        return NULL;
    }
    size_t buckets = (capacity > 0) ? capacity : DS_HASHMAP_INIT_BUCKETS;
    map->buckets = (ds_hashmap_entry_t**)ds_malloc(sizeof(ds_hashmap_entry_t*) * buckets);
    if (!map->buckets) {
        ds_free(map);
        ds_log(DS_LOG_ERROR, "[ds_hashmap] バケット配列確保失敗\n");
        return NULL;
    }
    for (size_t i = 0; i < buckets; ++i) map->buckets[i] = NULL;
    map->num_buckets = buckets;
    map->size = 0;
    map->key_free = key_free;
    map->val_free = val_free;
    return map;
}

// 破棄
ds_error_t ds_hashmap_destroy(ds_hashmap_t* map) {
    if (!map) return DS_ERR_NULL_POINTER;
    for (size_t i = 0; i < map->num_buckets; ++i) {
        ds_hashmap_entry_t* e = map->buckets[i];
        while (e) {
            ds_hashmap_entry_t* next = e->next;
            if (map->key_free) map->key_free(e->key);
            if (map->val_free) map->val_free(e->value);
            ds_free(e);
            e = next;
        }
    }
    ds_free(map->buckets);
    ds_free(map);
    return DS_SUCCESS;
}

// put
ds_error_t ds_hashmap_put(ds_hashmap_t* map, const char* key, void* value) {
    if (!map || !key) return DS_ERR_NULL_POINTER;
    unsigned long idx = ds_hashmap_hash(key) % map->num_buckets;
    ds_hashmap_entry_t* e = map->buckets[idx];
    while (e) {
        if (strcmp(e->key, key) == 0) {
            if (map->val_free) map->val_free(e->value);
            e->value = value;
            return DS_SUCCESS;
        }
        e = e->next;
    }
    // 新規
    e = ds_hashmap_new_entry(key, value);
    if (!e) return DS_ERR_ALLOC;
    e->next = map->buckets[idx];
    map->buckets[idx] = e;
    map->size++;
    return DS_SUCCESS;
}

// get
ds_error_t ds_hashmap_get(ds_hashmap_t* map, const char* key, void** out_value) {
    if (!map || !key || !out_value) return DS_ERR_NULL_POINTER;
    unsigned long idx = ds_hashmap_hash(key) % map->num_buckets;
    ds_hashmap_entry_t* e = map->buckets[idx];
    while (e) {
        if (strcmp(e->key, key) == 0) {
            *out_value = e->value;
            return DS_SUCCESS;
        }
        e = e->next;
    }
    *out_value = NULL;
    return DS_ERR_NOT_FOUND;
}

// remove
ds_error_t ds_hashmap_remove(ds_hashmap_t* map, const char* key) {
    if (!map || !key) return DS_ERR_NULL_POINTER;
    unsigned long idx = ds_hashmap_hash(key) % map->num_buckets;
    ds_hashmap_entry_t** prev = &map->buckets[idx];
    ds_hashmap_entry_t* e = map->buckets[idx];
    while (e) {
        if (strcmp(e->key, key) == 0) {
            *prev = e->next;
            if (map->key_free) map->key_free(e->key);
            if (map->val_free) map->val_free(e->value);
            ds_free(e);
            map->size--;
            return DS_SUCCESS;
        }
        prev = &e->next;
        e = e->next;
    }
    return DS_ERR_NOT_FOUND;
}

// サイズ
size_t ds_hashmap_size(const ds_hashmap_t* map) {
    return map ? map->size : 0;
}
