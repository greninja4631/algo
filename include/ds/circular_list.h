#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H

#include "../data_structures.h"
#include <stddef.h>
#include <stdbool.h>


// 円環リストの本体型（詳細は実装側で隠蔽）
typedef struct ds_circular_list ds_circular_list_t;

// --- API宣言 ---
/**
 * @brief 円環リスト生成
 */
ds_circular_list_t* ds_circular_list_create(void);

/**
 * @brief 円環リスト破棄
 */
ds_error_t ds_circular_list_destroy(ds_circular_list_t* list);

/**
 * @brief 末尾に要素を追加
 */
ds_error_t ds_circular_list_insert(ds_circular_list_t* list, void* data);

/**
 * @brief 先頭の要素を削除（out: dataに詰める）
 */
ds_error_t ds_circular_list_remove(ds_circular_list_t* list, void** data);

/**
 * @brief 空かどうか
 */
bool ds_circular_list_is_empty(const ds_circular_list_t* list);

/**
 * @brief 要素数
 */
size_t ds_circular_list_size(const ds_circular_list_t* list);

#endif // CIRCULAR_LIST_H
