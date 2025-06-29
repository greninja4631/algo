#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include "../data_structures.h"// 共通型（ds_error_t など）を使うため

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file queue.h
 * @brief 汎用キュー（FIFO, void*型）API
 */

// 本体構造体の前方宣言
typedef struct ds_queue ds_queue_t;

// ---------- API宣言 ----------

// キュー生成
ds_queue_t* ds_queue_create(void);

// 破棄
ds_error_t ds_queue_destroy(ds_queue_t* queue);

// エンキュー（末尾追加）
ds_error_t ds_queue_enqueue(ds_queue_t* queue, void* data);

// デキュー（先頭取り出し）
ds_error_t ds_queue_dequeue(ds_queue_t* queue, void** out_data);

// 先頭データ取得（削除しない）
ds_error_t ds_queue_front(const ds_queue_t* queue, void** out_data);

// 空判定
bool ds_queue_is_empty(const ds_queue_t* queue);

// サイズ取得
size_t ds_queue_size(const ds_queue_t* queue);

// 統計情報取得（必要に応じて）
ds_error_t ds_queue_get_stats(const ds_queue_t* queue, ds_stats_t* stats);

#ifdef __cplusplus
}
#endif

#endif // DS_QUEUE_H
