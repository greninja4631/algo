<<<<<<< HEAD
#include "logger.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "../../include/data_structures.h"
#include "../../include/ds/stack.h"          // ← 追加！！！！
#include "../../include/ds/rpn_calculator.h"
=======
=======
>>>>>>> feature
#include <stdbool.h>
#include "data_structures.h"
>>>>>>> feature
=======
#include "ds/rpn_calculator.h"   // API宣言のみ
#include "ds/stack.h"            // 依存モジュール
#include "util/logger.h"         // ログAPI
#include "util/memory.h"         // 差し替え用メモリアロケータ（※ds_malloc, ds_freeを使う！）

>>>>>>> feature
#include <stdlib.h>
#include <string.h>
#include <math.h>
<<<<<<< HEAD
<<<<<<< HEAD


// ---- 構造体本体 ----
struct ds_rpn_calculator
{
    ds_stack_t *stack;
};

// ---- 補助関数 ----
static int is_number(const char *str, double *out_val)
{
    char *endptr = NULL;
=======
#include <stdarg.h>
=======
>>>>>>> feature

/* --- 構造体本体は.cに完全隠蔽 --- */
struct ds_rpn_calculator {
    ds_stack_t* stack;
};

/* --- 内部ユーティリティ関数 --- */
static int is_number(const char* str, double* out_val) {
    char* endptr = NULL;
>>>>>>> feature
    *out_val = strtod(str, &endptr);
    return (endptr != str && *endptr == '\0');
}

/* --- API実装 --- */

<<<<<<< HEAD
<<<<<<< HEAD
ds_rpn_calculator_t *ds_rpn_calculator_create(void)
{
    ds_rpn_calculator_t *calc = (ds_rpn_calculator_t *)ds_malloc(sizeof(ds_rpn_calculator_t));
    if (!calc)
    {
        ds_log(DS_LOG_ERROR, "[ds_rpn_calculator_create] Memory allocation failed\n");
        return NULL;
    }
    calc->stack = ds_stack_create();
    if (!calc->stack)
    {
        ds_free(calc);
        ds_log(DS_LOG_ERROR, "[ds_rpn_calculator_create] Stack allocation failed\n");
=======
ds_rpn_calculator_t* ds_rpn_calculator_create(void) {
    ds_rpn_calculator_t* calc = (ds_rpn_calculator_t*)ds_malloc(sizeof(ds_rpn_calculator_t));
    if (!calc) return NULL;
    calc->stack = ds_stack_create();
    if (!calc->stack) {
        ds_free(calc);
>>>>>>> feature
        return NULL;
    }
    return calc;
=======
/**
 * @ownership: out_calcの所有権は呼び出し元
 */
ds_error_t ds_rpn_calculator_create(ds_rpn_calculator_t** out_calc) {
    if (!out_calc) return DS_ERR_NULL_POINTER;
    ds_rpn_calculator_t* calc = (ds_rpn_calculator_t*)ds_malloc(sizeof(ds_rpn_calculator_t));
    if (!calc) return DS_ERR_ALLOC;
    ds_error_t err = ds_stack_create(&calc->stack);
    if (err != DS_SUCCESS) { ds_free(calc); return err; }
    *out_calc = calc;
    return DS_SUCCESS;
>>>>>>> feature
}

<<<<<<< HEAD
ds_error_t ds_rpn_calculator_destroy(ds_rpn_calculator_t *calc)
{
    if (!calc)
        return DS_ERR_NULL_POINTER;
=======
ds_error_t ds_rpn_calculator_destroy(ds_rpn_calculator_t* calc) {
    if (!calc) return DS_ERR_NULL_POINTER;
>>>>>>> feature
    ds_stack_destroy(calc->stack);
    ds_free(calc);
    return DS_SUCCESS;
}

<<<<<<< HEAD
ds_error_t ds_rpn_calculator_reset(ds_rpn_calculator_t *calc)
{
    if (!calc)
        return DS_ERR_NULL_POINTER;
    return ds_stack_reset(calc->stack);
}

ds_error_t ds_rpn_calculator_push(ds_rpn_calculator_t *calc, double value)
{
    if (!calc)
        return DS_ERR_NULL_POINTER;
    double *val_ptr = (double *)ds_malloc(sizeof(double));
    if (!val_ptr)
    {
        ds_log(DS_LOG_ERROR, "[ds_rpn_calculator_push] Memory allocation failed\n");
        return DS_ERR_ALLOC;
    }
=======
ds_error_t ds_rpn_calculator_reset(ds_rpn_calculator_t* calc) {
    if (!calc) return DS_ERR_NULL_POINTER;
    return ds_stack_reset(calc->stack);
}

ds_error_t ds_rpn_calculator_push(ds_rpn_calculator_t* calc, double value) {
    if (!calc) return DS_ERR_NULL_POINTER;
    double* val_ptr = (double*)ds_malloc(sizeof(double));
    if (!val_ptr) return DS_ERR_ALLOC;
>>>>>>> feature
    *val_ptr = value;
    return ds_stack_push(calc->stack, val_ptr);
}

<<<<<<< HEAD
<<<<<<< HEAD
ds_error_t ds_rpn_calculator_pop(ds_rpn_calculator_t *calc, double *out)
{
    if (!calc || !out)
        return DS_ERR_NULL_POINTER;
    void *ptr = NULL;
    ds_error_t err = ds_stack_pop(calc->stack, &ptr);
    if (err != DS_SUCCESS)
        return err;
    if (!ptr)
        return DS_ERR_EMPTY;
    *out = *(double *)ptr;
=======
ds_error_t ds_rpn_calculator_pop(ds_rpn_calculator_t* calc, double* out) {
    if (!calc || !out) return DS_ERR_NULL_POINTER;
=======
ds_error_t ds_rpn_calculator_pop(ds_rpn_calculator_t* calc, double* out_value) {
    if (!calc || !out_value) return DS_ERR_NULL_POINTER;
>>>>>>> feature
    void* ptr = NULL;
    ds_error_t err = ds_stack_pop(calc->stack, &ptr);
    if (err != DS_SUCCESS) return err;
    if (!ptr) return DS_ERR_EMPTY;
<<<<<<< HEAD
    *out = *(double*)ptr;
>>>>>>> feature
=======
    *out_value = *(double*)ptr;
>>>>>>> feature
    ds_free(ptr);
    return DS_SUCCESS;
}

<<<<<<< HEAD
// --- RPN式評価 ---
<<<<<<< HEAD
ds_error_t ds_rpn_calculator_evaluate(ds_rpn_calculator_t *calc, const char *expression, double *result)
{
    if (!calc || !expression || !result)
        return DS_ERR_INVALID_ARG;
    ds_error_t err = DS_SUCCESS;
    ds_rpn_calculator_reset(calc);

    char *expr_copy = strdup(expression);
    if (!expr_copy)
        return DS_ERR_ALLOC;

    char *token = strtok(expr_copy, " ");
    while (token)
    {
        double value;
        if (is_number(token, &value))
        {
            if ((err = ds_rpn_calculator_push(calc, value)) != DS_SUCCESS)
                break;
        }
        else if (strlen(token) == 1 && strchr("+-*/", token[0]))
        {
            double rhs, lhs;
            if ((err = ds_rpn_calculator_pop(calc, &rhs)) != DS_SUCCESS)
                break;
            if ((err = ds_rpn_calculator_pop(calc, &lhs)) != DS_SUCCESS)
                break;

            double result_val = 0;
            switch (token[0])
            {
            case '+':
                result_val = lhs + rhs;
                break;
            case '-':
                result_val = lhs - rhs;
                break;
            case '*':
                result_val = lhs * rhs;
                break;
            case '/':
                if (rhs == 0)
                {
                    err = DS_ERR_INVALID_ARG;
                    break;
                }
                result_val = lhs / rhs;
                break;
            default:
                err = DS_ERR_INVALID_ARG;
                break;
            }
            if (err != DS_SUCCESS)
                break;
            if ((err = ds_rpn_calculator_push(calc, result_val)) != DS_SUCCESS)
                break;
        }
        else
        {
=======
ds_error_t ds_rpn_calculator_evaluate(ds_rpn_calculator_t* calc, const char* expression, double* result) {
    if (!calc || !expression || !result) return DS_ERR_INVALID_ARG;
    ds_error_t err = DS_SUCCESS;
    ds_rpn_calculator_reset(calc);
=======
/**
 * @param[in]  calc
 * @param[in]  expression 空白区切りのRPN式
 * @param[out] out_result 結果（エラー時は未定義）
 * @ownership: out_resultの所有権は呼び出し元
 */
ds_error_t ds_rpn_calculator_evaluate(ds_rpn_calculator_t* calc, const char* expression, double* out_result) {
    if (!calc || !expression || !out_result) return DS_ERR_INVALID_ARG;
    ds_error_t err = ds_rpn_calculator_reset(calc);
    if (err != DS_SUCCESS) return err;
>>>>>>> feature

    char* expr_copy = strdup(expression);
    if (!expr_copy) return DS_ERR_ALLOC;

    char* token = strtok(expr_copy, " ");
    while (token) {
        double value;
        if (is_number(token, &value)) {
            err = ds_rpn_calculator_push(calc, value);
        } else if (strlen(token) == 1 && strchr("+-*/", token[0])) {
            double rhs, lhs;
            err = ds_rpn_calculator_pop(calc, &rhs);
            if (err != DS_SUCCESS) break;
            err = ds_rpn_calculator_pop(calc, &lhs);
            if (err != DS_SUCCESS) break;

            double result_val = 0;
            switch (token[0]) {
                case '+': result_val = lhs + rhs; break;
                case '-': result_val = lhs - rhs; break;
                case '*': result_val = lhs * rhs; break;
                case '/':
                    if (rhs == 0) {
                        ds_log(DS_LOG_LEVEL_ERROR, "Division by zero in RPN expression!");
                        err = DS_ERR_INVALID_ARG;
                        break;
                    }
                    result_val = lhs / rhs;
                    break;
                default:
                    ds_log(DS_LOG_LEVEL_ERROR, "Unknown operator: %s", token);
                    err = DS_ERR_INVALID_ARG;
                    break;
            }
            if (err != DS_SUCCESS) break;
            err = ds_rpn_calculator_push(calc, result_val);
        } else {
<<<<<<< HEAD
>>>>>>> feature
=======
            ds_log(DS_LOG_LEVEL_ERROR, "Invalid token in RPN expression: %s", token);
>>>>>>> feature
            err = DS_ERR_INVALID_ARG;
        }
        if (err != DS_SUCCESS) break;
        token = strtok(NULL, " ");
    }

<<<<<<< HEAD
    if (err == DS_SUCCESS)
    {
        double final;
        if ((err = ds_rpn_calculator_pop(calc, &final)) == DS_SUCCESS)
        {
=======
    if (err == DS_SUCCESS) {
        double final;
<<<<<<< HEAD
        if ((err = ds_rpn_calculator_pop(calc, &final)) == DS_SUCCESS) {
>>>>>>> feature
            *result = final;
        }
=======
        err = ds_rpn_calculator_pop(calc, &final);
        if (err == DS_SUCCESS) *out_result = final;
>>>>>>> feature
    }
    free(expr_copy);
    return err;
<<<<<<< HEAD
}
=======
}
>>>>>>> feature
