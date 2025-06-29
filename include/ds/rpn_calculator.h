#ifndef RPN_CALCULATOR_H
#define RPN_CALCULATOR_H

#include <stddef.h>
#include "../data_structures.h" // 共通型（ds_error_tなど）

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif // RPN_CALCULATOR_H
