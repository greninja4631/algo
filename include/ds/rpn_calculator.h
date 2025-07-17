#ifndef DS_RPN_CALCULATOR_H
#define DS_RPN_CALCULATOR_H

#include <stddef.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file ds/rpn_calculator.h
 * @brief 逆ポーランド記法（RPN）電卓 Opaque型＋DIアロケータ設計
 *
 * 本体実装は src/ds/rpn_calculator.c のみに隠蔽
 */

//--- API ---

/**
 * @brief RPN電卓インスタンス生成
 * @param[in]  alloc      アロケータ
 * @param[out] out_calc   生成したインスタンス
 * @return DS_SUCCESS等
 * @ownership caller frees (destroy)
 */
ds_error_t ds_rpn_calculator_create(
    const ds_allocator_t* alloc,
    ds_rpn_calculator_t** out_calc
);

/**
 * @brief 電卓インスタンス破棄
 * @param[in] alloc    アロケータ
 * @param[in] calc     インスタンス
 */
ds_error_t ds_rpn_calculator_destroy(
    const ds_allocator_t* alloc,
    ds_rpn_calculator_t* calc
);

/**
 * @brief スタック状態リセット
 */
ds_error_t ds_rpn_calculator_reset(
    const ds_allocator_t* alloc,
    ds_rpn_calculator_t* calc
);

/**
 * @brief 値プッシュ
 */
ds_error_t ds_rpn_calculator_push(
    const ds_allocator_t* alloc,
    ds_rpn_calculator_t* calc,
    double value
);

/**
 * @brief 値ポップ（out_valueに取得）
 */
ds_error_t ds_rpn_calculator_pop(
    const ds_allocator_t* alloc,
    ds_rpn_calculator_t* calc,
    double* out_value
);

/**
 * @brief RPN式をそのまま評価・計算
 * @param[in]  alloc
 * @param[in]  calc
 * @param[in]  expression 空白区切りRPN式（例: "3 4 +"）
 * @param[out] out_result 計算結果
 * @return ds_error_t
 */
ds_error_t ds_rpn_calculator_evaluate(
    const ds_allocator_t* alloc,
    ds_rpn_calculator_t* calc,
    const char* expression,
    double* out_result
);

#ifdef __cplusplus
}
#endif
#endif // DS_RPN_CALCULATOR_H
