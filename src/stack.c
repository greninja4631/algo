// src/ds/stack.c

#include "ds/stack.h"           // 必ずAPIヘッダだけ（forward宣言/型/関数は全てdata_structures.hで管理）
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
// ---- 内部本体型定義（.c内のみ/外部非公開） ----
struct ds_stack {
    void **data;
    size_t capacity;
    size_t size;
};


#define DS_STACK_INIT_CAPACITY 8

// ---- API実装 ----

ds_error_t ds_stack_create(ds_stack_t **out_stack) {
    if (!out_stack) return DS_ERR_NULL_POINTER;

    ds_stack_t *s = (ds_stack_t*)malloc(sizeof(ds_stack_t));
    if (!s) return DS_ERR_ALLOC;

    s->data = (void**)malloc(DS_STACK_INIT_CAPACITY * sizeof(void*));
    if (!s->data) {
        free(s);
        return DS_ERR_ALLOC;
    }
    s->capacity = DS_STACK_INIT_CAPACITY;
    s->size = 0;
    *out_stack = s;
    return DS_SUCCESS;
}

ds_error_t ds_stack_destroy(ds_stack_t *stack) {
    if (!stack) return DS_ERR_NULL_POINTER;
    free(stack->data);
    free(stack);
    return DS_SUCCESS;
}

ds_error_t ds_stack_push(ds_stack_t *stack, void *data) {
    if (!stack) return DS_ERR_NULL_POINTER;
    if (stack->size >= stack->capacity) {
        size_t newcap = stack->capacity * 2;
        void **newdata = (void**)malloc(newcap * sizeof(void*));
        if (!newdata) return DS_ERR_ALLOC;
        for (size_t i = 0; i < stack->size; ++i)
            newdata[i] = stack->data[i];
        free(stack->data);
        stack->data = newdata;
        stack->capacity = newcap;
    }
    stack->data[stack->size++] = data;
    return DS_SUCCESS;
}

ds_error_t ds_stack_pop(ds_stack_t *stack, void **out_data) {
    if (!stack || !out_data) return DS_ERR_NULL_POINTER;
    if (stack->size == 0) return DS_ERR_EMPTY;
    *out_data = stack->data[--stack->size];
    return DS_SUCCESS;
}

ds_error_t ds_stack_peek(const ds_stack_t *stack, void **out_data) {
    if (!stack || !out_data) return DS_ERR_NULL_POINTER;
    if (stack->size == 0) return DS_ERR_EMPTY;
    *out_data = stack->data[stack->size - 1];
    return DS_SUCCESS;
}

bool ds_stack_is_empty(const ds_stack_t *stack) {
    return (!stack || stack->size == 0);
}

size_t ds_stack_size(const ds_stack_t *stack) {
    return stack ? stack->size : 0;
}

ds_error_t ds_stack_reset(ds_stack_t *stack) {
    if (!stack) return DS_ERR_NULL_POINTER;
    stack->size = 0;
    return DS_SUCCESS;
}

// ---- Stats API（本体参照はこの.c内だけ。外部からはAPI経由で一元化） ----
ds_error_t ds_stack_get_stats(const ds_stack_t *stack, ds_stats_t *stats) {
    if (!stack || !stats) return DS_ERR_NULL_POINTER;
    stats->total_elements      = stack->size;
    stats->memory_allocated    = stack->capacity * sizeof(void*) + sizeof(ds_stack_t);
    stats->operations_count    = 0;    // 必要なら実装拡張
    stats->creation_timestamp  = 0;    // 拡張用
    return DS_SUCCESS;
}