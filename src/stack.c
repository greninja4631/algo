#include "include/data_structures.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

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
        if (log_func) log_func(2, "stack: allocation failed"); //「log_func に何もセットされてなければ（NULL なら）何もしない」「log_func に関数がセットされていれば（NULL じゃなければ）その関数を呼び出す」 「log_func が指している関数」を、引数付きで呼び出す という意味
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

bool ds_stack_is_empty(const ds_stack_t* stack) {
    return (!stack || stack->size == 0);
}

size_t ds_stack_size(const ds_stack_t* stack) {
    return stack ? stack->size : 0;
}