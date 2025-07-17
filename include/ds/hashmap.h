<<<<<<< HEAD
#ifndef HASHMAP_H
#define HASHMAP_H
=======
#ifndef DS_HASHMAP_H
#define DS_HASHMAP_H
>>>>>>> feature

#include <stddef.h>
<<<<<<< HEAD
#include "data_structures.h"
<<<<<<< HEAD

// --- キーや値の解放用カスタム関数ポインタ ---
typedef void (*ds_hashmap_free_func)(void*);
=======
>>>>>>> feature

#ifdef __cplusplus
extern "C" {             /* C++ 互換 */
#endif
=======
#include <stdbool.h>
#include "data_structures.h"          /* ds_error_t / ds_allocator_t */
>>>>>>> feature

<<<<<<< HEAD
// --- 不透明型宣言（本体は.cで定義） ---
typedef struct ds_hashmap ds_hashmap_t;

<<<<<<< HEAD
// --- APIプロトタイプ ---
ds_hashmap_t* ds_hashmap_create(size_t capacity, ds_hashmap_free_func key_free, ds_hashmap_free_func val_free);
=======
ds_error_t ds_hashmap_create(size_t capacity, ds_hashmap_free_func key_free, ds_hashmap_free_func val_free, ds_hashmap_t** out_map);
>>>>>>> feature
=======

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

<<<<<<< HEAD
>>>>>>> feature
ds_error_t ds_hashmap_destroy(ds_hashmap_t* map);
ds_error_t ds_hashmap_put    (ds_hashmap_t* map, const char* key, void* value);
ds_error_t ds_hashmap_get    (ds_hashmap_t* map, const char* key, void** out_val);
ds_error_t ds_hashmap_remove (ds_hashmap_t* map, const char* key);
size_t     ds_hashmap_size   (const ds_hashmap_t* map);

#ifdef __cplusplus
}  /* extern "C" */
#endif
<<<<<<< HEAD

#endif // HASHMAP_H
=======
#endif /* DS_HASHMAP_H */
>>>>>>> feature
=======
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
>>>>>>> feature
