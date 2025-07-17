/**
 * @file    search.h
 * @brief   配列の線形探索・二分探索API（純粋関数・副作用ゼロ・アロケータ非依存）
 *
 * - 配列in-place探索のみ。外部メモリ割当不要（DI不要）。
 * - Opaque型ガイドライン/ソフトウェア工学的ベストプラクティス遵守。
 */
#ifndef DS_ALGO_SEARCH_H
#define DS_ALGO_SEARCH_H

#include <stddef.h>          // size_t, ptrdiff_t
#include "data_structures.h" // ds_error_t

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 線形探索
 * @param arr      [in] 配列ポインタ
 * @param n        [in] 配列長
 * @param target   [in] 探索値
 * @param out_index[out] 見つかった場合のインデックス（0-origin）。失敗時は -1
 * @return         DS_SUCCESS:成功/DS_ERR_NOT_FOUND:未発見/他
 * @note           arr==NULLまたはout_index==NULL時はDS_ERR_NULL_POINTER
 */
ds_error_t ds_search_linear(const int* arr, size_t n, int target, ptrdiff_t* out_index);

/**
 * @brief 二分探索（配列は昇順ソート済み前提）
 * @param arr      [in] 配列ポインタ
 * @param n        [in] 配列長
 * @param target   [in] 探索値
 * @param out_index[out] 見つかった場合のインデックス（0-origin）。失敗時は -1
 * @return         DS_SUCCESS:成功/DS_ERR_NOT_FOUND:未発見/他
 * @note           arr==NULLまたはout_index==NULL時はDS_ERR_NULL_POINTER
 */
ds_error_t ds_search_binary(const int* arr, size_t n, int target, ptrdiff_t* out_index);

#ifdef __cplusplus
}
#endif

#endif // DS_ALGO_SEARCH_H
