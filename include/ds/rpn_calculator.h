#ifndef DS_RPN_CALCULATOR_H
#define DS_RPN_CALCULATOR_H

#include <stddef.h>
#include "data_structures.h"     // ds_error_t, ds_stack_t, etc

#ifdef __cplusplus
extern "C" {
#endif


// --- API ---

/**
 * @brief RPN電卓インスタンス生成
 * @param[out] out_calc 生成した電卓インスタンスへのポインタ
 * @return ds_error_t
 * @ownership: out_calcの所有権は呼び出し元
 */
ds_error_t ds_rpn_calculator_create(ds_rpn_calculator_t** out_calc);

/**
 * @brief 破棄・メモリ解放
 */
ds_error_t ds_rpn_calculator_destroy(ds_rpn_calculator_t* calc);

/**
 * @brief 計算状態リセット
 */
ds_error_t ds_rpn_calculator_reset(ds_rpn_calculator_t* calc);

/**
 * @brief 値プッシュ
 */
ds_error_t ds_rpn_calculator_push(ds_rpn_calculator_t* calc, double value);

/**
 * @brief 値ポップ（out_valueに取得）
 */
ds_error_t ds_rpn_calculator_pop(ds_rpn_calculator_t* calc, double* out_value);

/**
 * @brief RPN式評価・計算
 * @param[in]  calc
 * @param[in]  expression 空白区切りのRPN式
 * @param[out] out_result 結果（エラー時は未定義）
 * @return ds_error_t
 */
ds_error_t ds_rpn_calculator_evaluate(ds_rpn_calculator_t* calc, const char* expression, double* out_result);

#ifdef __cplusplus
}
#endif
#endif // DS_RPN_CALCULATOR_H