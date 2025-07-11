/**
 * @file search.h
 * @brief 線形探索・二分探索API（サンプル）
 */
#ifndef DS_ALGO_SEARCH_H
#define DS_ALGO_SEARCH_H

#include <stddef.h>
#include "data_structures.h"  // ds_error_t など共通型

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 線形探索
 * @param arr 配列ポインタ
 * @param n   配列長
 * @param target 探索値
 * @param out_index 見つかった場合の添字（0-origin）。失敗時は -1
 * @return ds_error_t
 */
ds_error_t ds_search_linear(const int* arr, size_t n, int target, ptrdiff_t* out_index);

/**
 * @brief 二分探索（事前にソート済みが前提）
 * @param arr 配列ポインタ
 * @param n   配列長
 * @param target 探索値
 * @param out_index 見つかった場合の添字（0-origin）。失敗時は -1
 * @return ds_error_t
 */
ds_error_t ds_search_binary(const int* arr, size_t n, int target, ptrdiff_t* out_index);

#ifdef __cplusplus
}
#endif

#endif // DS_ALGO_SEARCH_H