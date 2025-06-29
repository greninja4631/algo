#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// メモリ確保/解放関数型
typedef void* (*ds_malloc_func_t)(size_t size);
typedef void  (*ds_free_func_t)(void* ptr);

// カスタム関数をセット（NULL指定でデフォルトに戻す）
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);

// 共通APIとして利用されるds_malloc/ds_free
void* ds_malloc(size_t size);
void  ds_free(void* ptr);

#ifdef __cplusplus
}
#endif

#endif // MEMORY_H
