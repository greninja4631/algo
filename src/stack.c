#include "logger.h"
#include <stdbool.h>
#include "data_structures.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct ds_stack {
    void **data;
    size_t capacity;
    size_t size;
};

#define STACK_INIT_CAPACITY 8

static ds_log_func_t log_func = NULL;
void ds_stack_set_log(ds_log_func_t logger) { log_func = logger; }

ds_stack_t* ds_stack_create(void) {
    ds_stack_t* s = malloc(sizeof(ds_stack_t));
    if (!s) {
        if (log_func) log_func(DS_LOG_ERROR, "stack: allocation failed");
        return NULL;
    }
    s->data = malloc(STACK_INIT_CAPACITY * sizeof(void*));
    if (!s->data) { free(s); return NULL; }
    s->capacity = STACK_INIT_CAPACITY;
    s->size = 0;
    return s;
}

ds_error_t ds_stack_destroy(ds_stack_t* stack) {
    if (!stack) return DS_ERR_NULL_POINTER;
    free(stack->data);
    free(stack);
    return DS_SUCCESS;
}

ds_error_t ds_stack_push(ds_stack_t* stack, void* data) {
    if (!stack) return DS_ERR_NULL_POINTER;
    if (stack->size >= stack->capacity) {
        size_t newcap = stack->capacity * 2;
        void** newdata = realloc(stack->data, newcap * sizeof(void*));
        if (!newdata) return DS_ERR_OUT_OF_MEMORY;
        stack->data = newdata;
        stack->capacity = newcap;
    }
    stack->data[stack->size++] = data;
    return DS_SUCCESS;
}

ds_error_t ds_stack_pop(ds_stack_t* stack, void** out_data) {
    if (!stack || !out_data) return DS_ERR_NULL_POINTER;
    if (stack->size == 0) return DS_ERR_EMPTY_CONTAINER;
    *out_data = stack->data[--stack->size];
    return DS_SUCCESS;
}

ds_error_t ds_stack_peek(const ds_stack_t* stack, void** out_data) {
    if (!stack || !out_data) return DS_ERR_NULL_POINTER;
    if (stack->size == 0) return DS_ERR_EMPTY_CONTAINER;
    *out_data = stack->data[stack->size - 1];
    return DS_SUCCESS;
}

bool ds_stack_is_empty(const ds_stack_t* stack) {
    return (!stack || stack->size == 0);
}

size_t ds_stack_size(const ds_stack_t* stack) {
    return stack ? stack->size : 0;
}

ds_error_t ds_stack_reset(ds_stack_t* stack) {
    if (!stack) return DS_ERR_NULL_POINTER;
    stack->size = 0;
    return DS_SUCCESS;
}

ds_error_t ds_stack_get_stats(const ds_stack_t* stack, ds_stats_t* stats) {
    if (!stack || !stats) return DS_ERR_NULL_POINTER;
    stats->total_elements = stack->size;
    stats->memory_allocated = stack->capacity * sizeof(void*);
    stats->operations_count = 0; // Optional: implement if you want operation count
    stats->creation_timestamp = 0; // Optional: implement if you want timestamp
    return DS_SUCCESS;
}