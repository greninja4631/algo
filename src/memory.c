#include "util/memory.h"
#include "data_structures.h"
#include <stdlib.h>

/* ---- グローバル状態：カスタムアロケータ関数ポインタ ---- */
static ds_malloc_func_t custom_malloc = NULL;
static ds_free_func_t   custom_free = NULL;

/**
 * @brief メモリ関数の登録（依存性注入。NULLで標準関数に戻る）
 */
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func) {
    custom_malloc = malloc_func;
    custom_free   = free_func;
}

/**
 * @brief メモリ確保ラッパー（外部から差し替え可・追跡もここで集中管理）
 */
void* ds_malloc(size_t size) {
    if (custom_malloc)
        return custom_malloc(size);
    return malloc(size);
}

/**
 * @brief メモリ解放ラッパー
 */
void ds_free(void* ptr) {
    if (custom_free)
        custom_free(ptr);
    else
        free(ptr);
}
