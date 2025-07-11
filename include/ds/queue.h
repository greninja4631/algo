#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include "data_structures.h"  // ds_queue_t, ds_error_t, 共通型

#ifdef __cplusplus
extern "C" {
#endif

#define DS_QUEUE_API_VERSION "1.0"

/**
 * @file
 * @brief 汎用キュー（FIFO）データ構造API
 */

/**
 * @brief キュー生成
 * @param[out] out_queue  生成されたキューインスタンスへのポインタ
 * @return                エラーコード
 */
ds_error_t ds_queue_create(ds_queue_t** out_queue);

/**
 * @brief キュー破棄
 * @param queue  インスタンス
 * @return       エラーコード
 */
ds_error_t ds_queue_destroy(ds_queue_t* queue);

/**
 * @brief キュー末尾に追加（enqueue）
 * @param queue  インスタンス
 * @param data   登録データ
 * @return       エラーコード
 */
ds_error_t ds_queue_enqueue(ds_queue_t* queue, void* data);

/**
 * @brief キュー先頭から取り出し（dequeue）
 * @param queue      インスタンス
 * @param[out] data  取り出したデータ（outポインタ）
 * @return           エラーコード
 */
ds_error_t ds_queue_dequeue(ds_queue_t* queue, void** data);

/**
 * @brief キュー先頭を覗く（popせず参照）
 * @param queue      インスタンス
 * @param[out] data  先頭データ（outポインタ）
 * @return           エラーコード
 */
ds_error_t ds_queue_front(const ds_queue_t* queue, void** data);

/**
 * @brief 空判定
 * @param queue  インスタンス
 * @return       true=空, false=要素あり
 */
bool ds_queue_is_empty(const ds_queue_t* queue);

/**
 * @brief 要素数取得
 * @param queue  インスタンス
 * @return       現在のサイズ
 */
size_t ds_queue_size(const ds_queue_t* queue);

#ifdef __cplusplus
}
#endif

#endif // DS_QUEUE_H