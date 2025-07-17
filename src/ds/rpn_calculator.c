<<<<<<< HEAD
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
=======
/*───────────────────────────────────────────────
 *  src/ds/rpn_calculator.c
 *  RPN (逆ポーランド) 電卓 ― 抽象アロケータ DI 対応
 *──────────────────────────────────────────────*/
#include "ds/rpn_calculator.h"
#include "ds/stack.h"
#include "util/logger.h"
#include "util/memory.h"

#include <string.h>   /* strtok, strlen */
#include <stdlib.h>   /* strtod         */
#include <math.h>     /* 数学演算       */
>>>>>>> feature

/* ───── 内部構造体 (Opaque 実体) ───── */
struct ds_rpn_calculator {
    const ds_allocator_t *alloc;   /* DI アロケータを保持 */
    ds_stack_t           *stack;   /* オペランドスタック */
};

<<<<<<< HEAD
/* --- 内部ユーティリティ関数 --- */
static int is_number(const char* str, double* out_val) {
    char* endptr = NULL;
>>>>>>> feature
    *out_val = strtod(str, &endptr);
    return (endptr != str && *endptr == '\0');
=======
/* ───── 内部ユーティリティ ───── */
static int
is_number(const char *s, double *out_val)
{
    char *end = NULL;
    *out_val  = strtod(s, &end);
    return (end != s && *end == '\0');
>>>>>>> feature
}

/*───────────────────────────────────────────────
 *                  API 実装
 *──────────────────────────────────────────────*/

<<<<<<< HEAD
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
=======
/* 生成 */
ds_error_t
ds_rpn_calculator_create(const ds_allocator_t *alloc,
                         ds_rpn_calculator_t **out_calc)
{
    if (!alloc || !out_calc) return DS_ERR_NULL_POINTER;

    ds_rpn_calculator_t *calc =
        (ds_rpn_calculator_t *)ds_malloc(alloc, 1, sizeof(ds_rpn_calculator_t));
>>>>>>> feature
    if (!calc) return DS_ERR_ALLOC;

    calc->alloc = alloc;
    ds_error_t rc = ds_stack_create(alloc, &calc->stack);
    if (rc != DS_SUCCESS) { ds_free(alloc, calc); return rc; }

    *out_calc = calc;
    return DS_SUCCESS;
>>>>>>> feature
}

<<<<<<< HEAD
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
=======
/* 破棄 */
ds_error_t
ds_rpn_calculator_destroy(const ds_allocator_t *alloc, ds_rpn_calculator_t *calc)
{
    if (!alloc || !calc) return DS_ERR_NULL_POINTER;

    ds_stack_destroy(alloc, calc->stack);
    ds_free(alloc, calc);
    return DS_SUCCESS;
}

/* リセット */
ds_error_t
ds_rpn_calculator_reset(ds_rpn_calculator_t *calc)
{
>>>>>>> feature
    if (!calc) return DS_ERR_NULL_POINTER;
    return ds_stack_reset(calc->alloc, calc->stack);
}

/* プッシュ */
ds_error_t
ds_rpn_calculator_push(ds_rpn_calculator_t *calc, double v)
{
    if (!calc) return DS_ERR_NULL_POINTER;
<<<<<<< HEAD
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
=======

    double *buf = (double *)ds_malloc(calc->alloc, 1, sizeof(double));
    if (!buf) return DS_ERR_ALLOC;

    *buf = v;
    return ds_stack_push(calc->alloc, calc->stack, buf);
}

/* ポップ */
ds_error_t
ds_rpn_calculator_pop(ds_rpn_calculator_t *calc, double *out_v)
{
    if (!calc || !out_v) return DS_ERR_NULL_POINTER;

    void *tmp = NULL;
    ds_error_t rc = ds_stack_pop(calc->alloc, calc->stack, &tmp);
    if (rc != DS_SUCCESS) return rc;

    *out_v = *(double *)tmp;
    ds_free(calc->alloc, tmp);
    return DS_SUCCESS;
}

/* 評価 */
ds_error_t
ds_rpn_calculator_evaluate(ds_rpn_calculator_t *calc,
                           const char          *expr,
                           double              *out_res)
{
    if (!calc || !expr || !out_res) return DS_ERR_INVALID_ARG;
>>>>>>> feature

    ds_error_t rc = ds_rpn_calculator_reset(calc);
    if (rc != DS_SUCCESS) return rc;

    /* strtok するために自前バッファを確保 */
    size_t len = strlen(expr) + 1;
    char  *buf = (char *)ds_malloc(calc->alloc, len, sizeof(char));
    if (!buf) return DS_ERR_ALLOC;
    memcpy(buf, expr, len);

    /* トークン走査 */
    for (char *tok = strtok(buf, " "); tok; tok = strtok(NULL, " ")) {

        double val = 0.0;
        if (is_number(tok, &val)) {
            rc = ds_rpn_calculator_push(calc, val);

        } else if (strlen(tok) == 1 && strchr("+-*/", tok[0])) {

            double rhs, lhs;
            rc = ds_rpn_calculator_pop(calc, &rhs);
            if (rc != DS_SUCCESS) break;
            rc = ds_rpn_calculator_pop(calc, &lhs);
            if (rc != DS_SUCCESS) break;

            double res = 0.0;
            switch (tok[0]) {
            case '+': res = lhs + rhs; break;
            case '-': res = lhs - rhs; break;
            case '*': res = lhs * rhs; break;
            case '/':
                if (rhs == 0.0) { rc = DS_ERR_INVALID_ARG; }
                else             res = lhs / rhs;
                break;
            }
            if (rc != DS_SUCCESS) break;
            rc = ds_rpn_calculator_push(calc, res);

        } else {
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature
=======
            ds_log(DS_LOG_LEVEL_ERROR, "Invalid token in RPN expression: %s", token);
>>>>>>> feature
            err = DS_ERR_INVALID_ARG;
=======
            rc = DS_ERR_INVALID_ARG;
>>>>>>> feature
        }

        if (rc != DS_SUCCESS) break;
    }

<<<<<<< HEAD
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
=======
    if (rc == DS_SUCCESS) {
        rc = ds_rpn_calculator_pop(calc, out_res);
    }

    ds_free(calc->alloc, buf);
    return rc;
}
>>>>>>> feature
