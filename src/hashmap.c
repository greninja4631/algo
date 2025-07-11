#include "ds/hashmap.h"
#include <stdlib.h>
#include <string.h>

/*───────────────────── 内部ノード ─────────────────────*/
typedef struct ds_hashmap_node {
    char*                   key;
    void*                   value;
    struct ds_hashmap_node* next;
} ds_hashmap_node_t;

/*───────────────────── 本体構造体 ─────────────────────*/
struct ds_hashmap {
    size_t                  capacity;
    size_t                  size;
    ds_hashmap_node_t**     buckets;
    ds_hashmap_free_func_t  key_free;   // コールバック型を *_func_t で統一
    ds_hashmap_free_func_t  val_free;
};

/*─────────────── FNV-1a 文字列ハッシュ ───────────────*/
static unsigned long _ds_hash_str(const char* s)
{
    unsigned long h = 2166136261u;
    for (; *s; ++s)
        h = (h ^ (unsigned char)*s) * 16777619u;
    return h;
}

/*───────────────────── API 実装 ─────────────────────*/
ds_error_t
ds_hashmap_create(size_t                  capacity,
                  ds_hashmap_free_func_t  key_free,
                  ds_hashmap_free_func_t  val_free,
                  ds_hashmap_t**          out_map)
{
    if (!out_map) return DS_ERR_NULL_POINTER;

    ds_hashmap_t* map = calloc(1, sizeof(*map));
    if (!map) return DS_ERR_ALLOC;

    map->capacity = capacity ? capacity : 16;
    map->buckets  = calloc(map->capacity, sizeof(ds_hashmap_node_t*));
    if (!map->buckets) { free(map); return DS_ERR_ALLOC; }

    map->key_free = key_free;
    map->val_free = val_free;
    *out_map      = map;
    return DS_SUCCESS;
}

ds_error_t
ds_hashmap_destroy(ds_hashmap_t* map)
{
    if (!map) return DS_ERR_NULL_POINTER;

    for (size_t i = 0; i < map->capacity; ++i) {
        for (ds_hashmap_node_t* n = map->buckets[i]; n; ) {
            ds_hashmap_node_t* nxt = n->next;
            if (map->key_free) map->key_free(n->key);
            if (map->val_free) map->val_free(n->value);
            free(n);
            n = nxt;
        }
    }
    free(map->buckets);
    free(map);
    return DS_SUCCESS;
}

ds_error_t
ds_hashmap_put(ds_hashmap_t* map, const char* key, void* value)
{
    if (!map || !key) return DS_ERR_NULL_POINTER;

    size_t idx = _ds_hash_str(key) % map->capacity;
    for (ds_hashmap_node_t* n = map->buckets[idx]; n; n = n->next)
        if (strcmp(n->key, key) == 0) return DS_ERR_INVALID_ARG; // Duplicate

    ds_hashmap_node_t* n = malloc(sizeof(*n));
    if (!n) return DS_ERR_ALLOC;

    n->key   = strdup(key);
    n->value = value;
    n->next  = map->buckets[idx];
    map->buckets[idx] = n;
    map->size++;
    return DS_SUCCESS;
}

ds_error_t
ds_hashmap_get(ds_hashmap_t* map, const char* key, void** out_val)
{
    if (!map || !key || !out_val) return DS_ERR_NULL_POINTER;

    size_t idx = _ds_hash_str(key) % map->capacity;
    for (ds_hashmap_node_t* n = map->buckets[idx]; n; n = n->next)
        if (strcmp(n->key, key) == 0) {
            *out_val = n->value;
            return DS_SUCCESS;
        }

    return DS_ERR_NOT_FOUND;
}

ds_error_t
ds_hashmap_remove(ds_hashmap_t* map, const char* key)
{
    if (!map || !key) return DS_ERR_NULL_POINTER;

    size_t idx = _ds_hash_str(key) % map->capacity;
    ds_hashmap_node_t *n = map->buckets[idx], *prev = NULL;

    while (n) {
        if (strcmp(n->key, key) == 0) {
            if (prev) prev->next = n->next;
            else      map->buckets[idx] = n->next;

            if (map->key_free) map->key_free(n->key);
            if (map->val_free) map->val_free(n->value);
            free(n);
            map->size--;
            return DS_SUCCESS;
        }
        prev = n;
        n    = n->next;
    }
    return DS_ERR_NOT_FOUND;
}

size_t
ds_hashmap_size(const ds_hashmap_t* map)
{
    return map ? map->size : 0;
}