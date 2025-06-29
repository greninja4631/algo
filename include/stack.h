#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- 不透明型宣言（実装はstack.c） ---
typedef struct ds_stack ds_stack_t;

// --- ユーザー向けAPIプロトタイプ ---
// ds_error_t, int32_t などは data_structures.h で定義されている想定

// スタック作成（capacityは将来拡張用、現状未使用可）
ds_stack_t* ds_stack_create(size_t capacity);

// 破棄
void ds_stack_destroy(ds_stack_t* stack);

// プッシュ（積む）
ds_error_t ds_stack_push(ds_stack_t* stack, int32_t value);

// ポップ（取り出す）
ds_error_t ds_stack_pop(ds_stack_t* stack, int32_t* out_value);

// スタックのサイズ取得
size_t ds_stack_size(const ds_stack_t* stack);

// 空かどうか
bool ds_stack_is_empty(const ds_stack_t* stack);

#ifdef __cplusplus
}
#endif

#endif // STACK_H