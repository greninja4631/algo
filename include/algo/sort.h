/**
 * @file sort.h
 * @brief 基本的なソートAPI（サンプル: バブル・挿入）
 */
#ifndef DS_ALGO_SORT_H
#define DS_ALGO_SORT_H

#include <stddef.h>
#include "data_structures.h"  // ds_error_t など

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief バブルソート
 * @param arr 配列
 * @param n   配列長
 * @return ds_error_t
 */
ds_error_t ds_sort_bubble(int* arr, size_t n);

/**
 * @brief 挿入ソート
 * @param arr 配列
 * @param n   配列長
 * @return ds_error_t
 */
ds_error_t ds_sort_insertion(int* arr, size_t n);

#ifdef __cplusplus
}
#endif

#endif // DS_ALGO_SORT_H




// algo/sort.h
// 	•	役割：ソート（並べ替え）アルゴリズムのAPI宣言（外部公開用インターフェース）
// 	•	例：バブルソート、挿入ソートなど
// 	•	利用例：
// 	•	配列を昇順や降順に並べ替えたいとき、ds_sort_bubbleなどの関数を利用
