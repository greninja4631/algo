#ifndef UTIL_MEMORY_H
#define UTIL_MEMORY_H

#include "data_structures.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// アロケータ登録
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);
// ラップ関数
void* ds_malloc(size_t size);
void  ds_free(void* ptr);

#ifdef __cplusplus
}
#endif

#endif // UTIL_MEMORY_H