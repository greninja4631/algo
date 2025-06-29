#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- 構造体本体の不透明型宣言（実装はqueue.c側で定義） ---
typedef struct ds_queue ds_queue_t;

// --- ユーザー向けAPIプロトタイプ ---
// ※エラー型（ds_error_t）や必要な型はdata_structures.hで定義されている想定

// キュー作成（capacityは将来拡張用、現状未使用可）
ds_queue_t* ds_queue_create(size_t capacity);

// 破棄
void ds_queue_destroy(ds_queue_t* queue);

// エンキュー（末尾に追加）
ds_error_t ds_queue_enqueue(ds_queue_t* queue, int32_t value);

// デキュー（先頭から取り出し）
ds_error_t ds_queue_dequeue(ds_queue_t* queue, int32_t* out_value);

// キューのサイズ取得
size_t ds_queue_size(const ds_queue_t* queue);

// 空かどうか
bool ds_queue_is_empty(const ds_queue_t* queue);

#ifdef __cplusplus
}
#endif

#endif // QUEUE_H