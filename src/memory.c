#include "util/memory.h"
#include "data_structures.h"
#include <stddef.h>
#include <stdlib.h>

/* グローバルDIアロケータ。NULL時は標準malloc/free */
static ds_malloc_func_t custom_malloc = NULL;
static ds_free_func_t   custom_free   = NULL;

void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func) {
    custom_malloc = malloc_func ? malloc_func : malloc;
    custom_free   = free_func   ? free_func   : free;
}

/* DI/標準/ゼロ初期化calloc全部に対応 */
void* ds_malloc(const ds_allocator_t* alloc, size_t count, size_t size) {
    if (alloc && alloc->alloc) {
        return alloc->alloc(count, size);
    }
    if (custom_malloc) {
        return custom_malloc(count * size);
    }
    return calloc(count, size);
}

void ds_free(const ds_allocator_t* alloc, void* ptr) {
    if (!ptr) return;
    if (alloc && alloc->free) {
        alloc->free(ptr);
        return;
    }
    if (custom_free) {
        custom_free(ptr);
        return;
    }
    free(ptr);
}
