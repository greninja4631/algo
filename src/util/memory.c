#include "../include/util/memory.h"
#include <stdlib.h>

// デフォルト関数
static void* default_malloc(size_t size) { return malloc(size); }
static void  default_free(void* ptr)     { free(ptr); }

// 関数ポインタ（初期値: デフォルト）
static ds_malloc_func_t g_malloc_func = default_malloc;
static ds_free_func_t   g_free_func   = default_free;

void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func) {
    g_malloc_func = malloc_func ? malloc_func : default_malloc;
    g_free_func   = free_func   ? free_func   : default_free;
}

void* ds_malloc(size_t size) { return g_malloc_func(size); }
void  ds_free(void* ptr)     { g_free_func(ptr); }
