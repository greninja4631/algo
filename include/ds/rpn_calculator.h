<<<<<<< HEAD
#ifndef RPN_CALCULATOR_H
#define RPN_CALCULATOR_H

#include <stddef.h>
#include "../data_structures.h" // 共通型（ds_error_tなど）
=======
#ifndef DS_RPN_CALCULATOR_H
#define DS_RPN_CALCULATOR_H

#include <stddef.h>
#include "data_structures.h"     // ds_error_t, ds_stack_t, etc
>>>>>>> feature

#ifdef __cplusplus
extern "C" {
#endif

<<<<<<< HEAD
typedef struct ds_rpn_calculator ds_rpn_calculator_t;

// RPN電卓の生成と破棄
ds_rpn_calculator_t* ds_rpn_calculator_create(void);
ds_error_t ds_rpn_calculator_destroy(ds_rpn_calculator_t* calc);

// スタッククリア
ds_error_t ds_rpn_calculator_reset(ds_rpn_calculator_t* calc);

// 値をプッシュ
ds_error_t ds_rpn_calculator_push(ds_rpn_calculator_t* calc, double value);

// 値をポップ
ds_error_t ds_rpn_calculator_pop(ds_rpn_calculator_t* calc, double* out);

// 文字列式を評価（例: "3 4 + 5 *" → 35.0）
ds_error_t ds_rpn_calculator_evaluate(ds_rpn_calculator_t* calc, const char* expression, double* result);
=======

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
>>>>>>> feature

#ifdef __cplusplus
}
#endif
<<<<<<< HEAD

#endif // RPN_CALCULATOR_H
=======
#endif // DS_RPN_CALCULATOR_H
>>>>>>> feature
