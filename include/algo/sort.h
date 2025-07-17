#ifndef DS_SORT_H
#define DS_SORT_H

#include <stddef.h>
#include "data_structures.h"  // ds_error_t など共通型

#ifdef __cplusplus
extern "C" {
#endif

#define DS_SORT_API_VERSION "1.0"

/**
 * @file
 * @brief 基本ソートアルゴリズムAPI（全関数ユニットテスト対応）
 *
 * - アルゴリズム本体は .c に隠蔽、APIのみ宣言
 * - 失敗時は ds_error_t を返す
 * - 配列ポインタはNULLチェックされる
 * - inplace（配列を書き換える）動作
 */

/**
 * @brief バブルソート
 * @param[in,out] arr ソート対象配列
 * @param[in] n 配列長
 * @return エラーコード
 */
ds_error_t ds_sort_bubble(int* arr, size_t n);

/**
 * @brief 挿入ソート
 * @param[in,out] arr ソート対象配列
 * @param[in] n 配列長
 * @return エラーコード
 */
ds_error_t ds_sort_insertion(int* arr, size_t n);

// 必要に応じ他アルゴリズムをここへ追加

#ifdef __cplusplus
}
#endif

#endif // DS_SORT_H
