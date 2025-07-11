<<<<<<< HEAD
#ifndef HASHMAP_H
#define HASHMAP_H
=======
#ifndef DS_HASHMAP_H
#define DS_HASHMAP_H
>>>>>>> feature

#include <stddef.h>
#include "data_structures.h"
<<<<<<< HEAD

// --- キーや値の解放用カスタム関数ポインタ ---
typedef void (*ds_hashmap_free_func)(void*);
=======
>>>>>>> feature

#ifdef __cplusplus
extern "C" {             /* C++ 互換 */
#endif

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

/* ─ コールバック型は *_func_t で統一 ─ */
typedef void (*ds_hashmap_free_func_t)(void*);

/* ─ API ─ */
ds_error_t ds_hashmap_create(size_t               capacity,
                             ds_hashmap_free_func_t key_free,
                             ds_hashmap_free_func_t val_free,
                             ds_hashmap_t**       out_map);

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
