#ifndef DS_QUEUE_H
#define DS_QUEUE_H
/**
 * @file    include/ds/queue.h
 * @brief   汎用 FIFO キュー（Opaque 型 + 抽象アロケータ DI）
 * @version 1.0
 */
#include <stddef.h>
#include <stdbool.h>
#include "data_structures.h"   /* ds_error_t / ds_allocator_t / ds_stats_t */

#ifdef __cplusplus
extern "C" {      /* C++ 互換 */
#endif


/*──────────── CRUD ────────────*/
/**
 * @brief キュー生成
 * @ownership caller-frees
 */
ds_error_t ds_queue_create(const ds_allocator_t *alloc, ds_queue_t **out_queue);

/**
 * @brief キュー破棄（NULL安全）
 * @ownership caller-frees
 */
ds_error_t ds_queue_destroy(const ds_allocator_t *alloc, ds_queue_t *queue);

/*──────────── 操作 ────────────*/

/**
 * @brief 末尾にデータを追加
 */
ds_error_t ds_queue_enqueue(const ds_allocator_t *alloc, ds_queue_t *queue, void *data);

/**
 * @brief 先頭からデータを取り出す
 */
ds_error_t ds_queue_dequeue(const ds_allocator_t *alloc, ds_queue_t *queue, void **out_data);

/*──────────── 参照・情報 ────────────*/

/**
 * @brief 先頭要素の参照（popしない）
 * @note 読み取り専用なのでalloc不要
 */
ds_error_t ds_queue_front(const ds_queue_t *queue, void **out_data);

bool   ds_queue_is_empty(const ds_queue_t *queue);
size_t ds_queue_size    (const ds_queue_t *queue);

ds_error_t ds_queue_get_stats(const ds_queue_t *queue, ds_stats_t *stats);

#ifdef __cplusplus
}
#endif
#endif /* DS_QUEUE_H */
