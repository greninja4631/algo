#ifndef DS_HASHMAP_H
#define DS_HASHMAP_H

#include <stddef.h>
#include <stdbool.h>
#include "data_structures.h"          /* ds_error_t / ds_allocator_t */


/* 要素破棄コールバック */
typedef void (*ds_hashmap_free_func_t)(void *ptr);

/* ───── Life-cycle ───── */
/**
 * @param alloc      抽象アロケータ (必須)
 * @param capacity   初期バケット数 (0 なら 16)
 * @param key_free   key 破棄関数 (NULL 可)
 * @param val_free   value 破棄関数 (NULL 可)
 * @param[out] out   生成されたハンドル
 */
ds_error_t ds_hashmap_create (const ds_allocator_t      *alloc,
                              size_t                     capacity,
                              ds_hashmap_free_func_t     key_free,
                              ds_hashmap_free_func_t     val_free,
                              ds_hashmap_t             **out);

/**
 * 要素をすべて破棄して map を解放。
 */
ds_error_t ds_hashmap_destroy(const ds_allocator_t *alloc,
                              ds_hashmap_t         *map);

/* ───── Mutation ───── */
ds_error_t ds_hashmap_put   (const ds_allocator_t *alloc,
                             ds_hashmap_t         *map,
                             const char           *key,
                             void                 *value);

ds_error_t ds_hashmap_remove(const ds_allocator_t *alloc,
                             ds_hashmap_t         *map,
                             const char           *key);

/* ───── Query ───── */
ds_error_t ds_hashmap_get (ds_hashmap_t *map,
                           const char   *key,
                           void        **out_value);

size_t     ds_hashmap_size(const ds_hashmap_t *map);

#endif /* DS_HASHMAP_H */
