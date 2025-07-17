/**
 * @file    src/ds/rpn_calculator.c
 * @brief   逆ポーランド記法（RPN）電卓 実装（ガイドライン完全準拠）
 */
#include "ds/rpn_calculator.h"
#include "util/memory.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*───── Opaque本体構造体（.c隠蔽）─────*/
struct ds_rpn_calculator {
    double *stack;
    size_t  size, capacity;
};

/*───── スタック確保初期サイズ─────*/
#define RPN_INIT_CAP 16

/*───── 内部ユーティリティ─────*/
static ds_error_t rpn_ensure_capacity(const ds_allocator_t *alloc, ds_rpn_calculator_t *calc, size_t min_cap) {
    if (calc->capacity >= min_cap) return DS_SUCCESS;
    size_t new_cap = calc->capacity ? calc->capacity * 2 : RPN_INIT_CAP;
    while (new_cap < min_cap) new_cap *= 2;
    double *new_stack = ds_realloc(alloc, calc->stack, new_cap * sizeof(double));
    if (!new_stack) return DS_ERR_ALLOC;
    calc->stack = new_stack;
    calc->capacity = new_cap;
    return DS_SUCCESS;
}

/*───── API実装 ─────*/

ds_error_t ds_rpn_calculator_create(
    const ds_allocator_t *alloc,
    ds_rpn_calculator_t **out_calc)
{
    if (!alloc || !out_calc) return DS_ERR_NULL_POINTER;
    ds_rpn_calculator_t *calc = ds_malloc(alloc, 1, sizeof(*calc));
    if (!calc) return DS_ERR_ALLOC;
    calc->stack = NULL;
    calc->size = 0;
    calc->capacity = 0;
    *out_calc = calc;
    return DS_SUCCESS;
}

ds_error_t ds_rpn_calculator_destroy(
    const ds_allocator_t *alloc,
    ds_rpn_calculator_t *calc)
{
    if (!alloc || !calc) return DS_ERR_NULL_POINTER;
    if (calc->stack) ds_free(alloc, calc->stack);
    ds_free(alloc, calc);
    return DS_SUCCESS;
}

ds_error_t ds_rpn_calculator_reset(
    const ds_allocator_t *alloc,
    ds_rpn_calculator_t *calc)
{
    (void)alloc;
    if (!calc) return DS_ERR_NULL_POINTER;
    calc->size = 0;
    return DS_SUCCESS;
}

ds_error_t ds_rpn_calculator_push(
    const ds_allocator_t *alloc,
    ds_rpn_calculator_t *calc,
    double v)
{
    if (!alloc || !calc) return DS_ERR_NULL_POINTER;
    ds_error_t rc = rpn_ensure_capacity(alloc, calc, calc->size + 1);
    if (rc != DS_SUCCESS) return rc;
    calc->stack[calc->size++] = v;
    return DS_SUCCESS;
}

ds_error_t ds_rpn_calculator_pop(
    const ds_allocator_t *alloc,
    ds_rpn_calculator_t *calc,
    double *out_v)
{
    if (!calc || !out_v) return DS_ERR_NULL_POINTER;
    if (calc->size == 0) return DS_ERR_EMPTY;
    *out_v = calc->stack[--calc->size];
    return DS_SUCCESS;
}

ds_error_t ds_rpn_calculator_evaluate(
    const ds_allocator_t *alloc,
    ds_rpn_calculator_t *calc,
    const char *expr,
    double *out_res)
{
    if (!alloc || !calc || !expr || !out_res) return DS_ERR_NULL_POINTER;
    ds_rpn_calculator_reset(alloc, calc);

    char buf[128];
    strncpy(buf, expr, sizeof(buf) - 1);
    buf[sizeof(buf)-1] = '\0';
    char *token = strtok(buf, " ");
    ds_error_t rc = DS_SUCCESS;

    while (token) {
        char *end;
        double val = strtod(token, &end);
        if (end != token) {
            rc = ds_rpn_calculator_push(alloc, calc, val);
        } else if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
                   strcmp(token, "*") == 0 || strcmp(token, "/") == 0) {
            double rhs, lhs;
            if ((rc = ds_rpn_calculator_pop(alloc, calc, &rhs)) != DS_SUCCESS) break;
            if ((rc = ds_rpn_calculator_pop(alloc, calc, &lhs)) != DS_SUCCESS) break;
            double res = 0.0;
            if (strcmp(token, "+") == 0) res = lhs + rhs;
            else if (strcmp(token, "-") == 0) res = lhs - rhs;
            else if (strcmp(token, "*") == 0) res = lhs * rhs;
            else if (strcmp(token, "/") == 0) res = lhs / rhs;
            rc = ds_rpn_calculator_push(alloc, calc, res);
        } else {
            rc = DS_ERR_INVALID_ARG;
            break;
        }
        token = strtok(NULL, " ");
    }
    if (rc == DS_SUCCESS && calc->size == 1) {
        rc = ds_rpn_calculator_pop(alloc, calc, out_res);
    } else if (rc == DS_SUCCESS) {
        rc = DS_ERR_INVALID_ARG;
    }
    return rc;
}