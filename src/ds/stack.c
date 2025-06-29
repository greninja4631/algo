#include "../../include/data_structures.h"
#include "../../include/ds/stack.h"
#include <stdlib.h>
#include <stdio.h>

// デフォルト初期容量
#define STACK_INIT_CAPACITY 8

//-----------------
// スタック構造体本体
//-----------------
struct ds_stack {
    void **data;
    size_t capacity;
    size_t size;
};

//-----------------
// スタック生成
//-----------------
ds_stack_t* ds_stack_create(void) {
    ds_stack_t* s = (ds_stack_t*)ds_malloc(sizeof(ds_stack_t));
    if (!s) {
        ds_log(DS_LOG_ERROR, "[stack] メモリ確保失敗\n");
        return NULL;
    }
    s->data = (void**)ds_malloc(STACK_INIT_CAPACITY * sizeof(void*));
    if (!s->data) {
        ds_log(DS_LOG_ERROR, "[stack] 配列確保失敗\n");
        ds_free(s);
        return NULL;
    }
    s->capacity = STACK_INIT_CAPACITY;
    s->size = 0;
    return s;
}

//-----------------
// スタック破棄
//-----------------
ds_error_t ds_stack_destroy(ds_stack_t* stack) {
    if (!stack) return DS_ERR_NULL_POINTER;
    ds_free(stack->data);
    ds_free(stack);
    return DS_SUCCESS;
}

//-----------------
// プッシュ
//-----------------
ds_error_t ds_stack_push(ds_stack_t* stack, void* data) {
    if (!stack) return DS_ERR_NULL_POINTER;
    if (stack->size >= stack->capacity) {
        size_t newcap = stack->capacity * 2;
        void** newdata = (void**)ds_malloc(newcap * sizeof(void*));
        if (!newdata) {
            ds_log(DS_LOG_ERROR, "[stack] 再確保失敗\n");
            return DS_ERR_OUT_OF_MEMORY;
        }
        for (size_t i = 0; i < stack->size; ++i)
            newdata[i] = stack->data[i];
        ds_free(stack->data);
        stack->data = newdata;
        stack->capacity = newcap;
    }
    stack->data[stack->size++] = data;
    return DS_SUCCESS;
}

//-----------------
// ポップ
//-----------------
ds_error_t ds_stack_pop(ds_stack_t* stack, void** out_data) {
    if (!stack || !out_data) return DS_ERR_NULL_POINTER;
    if (stack->size == 0) return DS_ERR_EMPTY_CONTAINER;
    *out_data = stack->data[--stack->size];
    return DS_SUCCESS;
}

//-----------------
// ピーク（先頭参照）
//-----------------
ds_error_t ds_stack_peek(const ds_stack_t* stack, void** out_data) {
    if (!stack || !out_data) return DS_ERR_NULL_POINTER;
    if (stack->size == 0) return DS_ERR_EMPTY_CONTAINER;
    *out_data = stack->data[stack->size - 1];
    return DS_SUCCESS;
}

//-----------------
// 空判定
//-----------------
bool ds_stack_is_empty(const ds_stack_t* stack) {
    return (!stack || stack->size == 0);
}

//-----------------
// サイズ取得
//-----------------
size_t ds_stack_size(const ds_stack_t* stack) {
    return stack ? stack->size : 0;
}

//-----------------
// スタックリセット
//-----------------
ds_error_t ds_stack_reset(ds_stack_t* stack) {
    if (!stack) return DS_ERR_NULL_POINTER;
    stack->size = 0;
    return DS_SUCCESS;
}

//-----------------
// 統計情報取得
//-----------------
ds_error_t ds_stack_get_stats(const ds_stack_t* stack, ds_stats_t* stats) {
    if (!stack || !stats) return DS_ERR_NULL_POINTER;
    stats->total_elements = stack->size;
    stats->memory_allocated = stack->capacity * sizeof(void*);
    stats->operations_count = 0;     // 必要ならpush/pop時にインクリメント管理
    stats->creation_timestamp = 0;   // 拡張用
    return DS_SUCCESS;
}
