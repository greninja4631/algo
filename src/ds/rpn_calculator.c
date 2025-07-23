#include <stddef.h>
/*======================================================================*
 *  src/ds/rpn_calculator.c
 *  RPN (逆ポーランド) 電卓 — Opaque型＋抽象アロケータDI設計
 *  2025-07 API設計・実装ガイドライン完全準拠
 *======================================================================*/
#include "ds/rpn_calculator.h"
#include "ds/stack.h"
#include "util/logger.h"
#include "util/memory.h"

#include <string.h>   /* strtok, strlen */
#include <stdlib.h>   /* strtod         */
#include <math.h>     /* 数学演算       */

/* ───── Opaque実体 ───── */
struct ds_rpn_calculator {
    ds_stack_t *stack; // DIでアロケータ渡し
};

/* ───── 内部ユーティリティ ───── */
static int
is_number(const char *s, double *out_val)
{
    char *end = NULL;
    *out_val  = strtod(s, &end);
    return (end != s && *end == '\0');
}

/*───────────────────────────────
 *             API 実装
 *───────────────────────────────*/

ds_error_t
ds_rpn_calculator_create(const ds_allocator_t *alloc,
                         ds_rpn_calculator_t **out_calc)
{
    if (!alloc || !out_calc) return DS_ERR_NULL_POINTER;
    ds_rpn_calculator_t *calc = ds_malloc(alloc, 1, sizeof(ds_rpn_calculator_t));
    if (!calc) return DS_ERR_ALLOC;

    ds_error_t rc = ds_stack_create(alloc, &calc->stack);
    if (rc != DS_SUCCESS) {
        ds_free(alloc, calc);
        return rc;
    }
    *out_calc = calc;
    return DS_SUCCESS;
}

ds_error_t
ds_rpn_calculator_destroy(const ds_allocator_t *alloc, ds_rpn_calculator_t *calc)
{
    if (!alloc || !calc) return DS_ERR_NULL_POINTER;
    ds_stack_destroy(alloc, calc->stack);
    ds_free(alloc, calc);
    return DS_SUCCESS;
}

ds_error_t
ds_rpn_calculator_reset(const ds_allocator_t *alloc, ds_rpn_calculator_t *calc)
{
    if (!alloc || !calc) return DS_ERR_NULL_POINTER;
    return ds_stack_reset(alloc, calc->stack);
}

ds_error_t
ds_rpn_calculator_push(const ds_allocator_t *alloc, ds_rpn_calculator_t *calc, double v)
{
    if (!alloc || !calc) return DS_ERR_NULL_POINTER;
    double *buf = ds_malloc(alloc, 1, sizeof(double));
    if (!buf) return DS_ERR_ALLOC;
    *buf = v;
    ds_error_t rc = ds_stack_push(alloc, calc->stack, buf);
    if (rc != DS_SUCCESS) ds_free(alloc, buf); // push失敗時のリーク防止
    return rc;
}

ds_error_t
ds_rpn_calculator_pop(const ds_allocator_t *alloc, ds_rpn_calculator_t *calc, double *out_v)
{
    if (!alloc || !calc || !out_v) return DS_ERR_NULL_POINTER;
    void *tmp = NULL;
    ds_error_t rc = ds_stack_pop(alloc, calc->stack, &tmp);
    if (rc != DS_SUCCESS) return rc;
    *out_v = *(double *)tmp;
    ds_free(alloc, tmp);
    return DS_SUCCESS;
}

ds_error_t
ds_rpn_calculator_evaluate(const ds_allocator_t *alloc,
                           ds_rpn_calculator_t *calc,
                           const char *expr,
                           double *out_res)
{
    if (!alloc || !calc || !expr || !out_res) return DS_ERR_INVALID_ARG;
    ds_error_t rc = ds_rpn_calculator_reset(alloc, calc);
    if (rc != DS_SUCCESS) return rc;

    size_t len = strlen(expr) + 1;
    char *buf = ds_malloc(alloc, len, sizeof(char));
    if (!buf) return DS_ERR_ALLOC;
    memcpy(buf, expr, len);

    for (char *tok = strtok(buf, " "); tok; tok = strtok(NULL, " ")) {
        double val = 0.0;
        if (is_number(tok, &val)) {
            rc = ds_rpn_calculator_push(alloc, calc, val);

        } else if (strlen(tok) == 1 && strchr("+-*/", tok[0])) {
            double rhs, lhs;
            rc = ds_rpn_calculator_pop(alloc, calc, &rhs);
            if (rc != DS_SUCCESS) break;
            rc = ds_rpn_calculator_pop(alloc, calc, &lhs);
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
            rc = ds_rpn_calculator_push(alloc, calc, res);

        } else {
            rc = DS_ERR_INVALID_ARG;
        }
        if (rc != DS_SUCCESS) break;
    }

    if (rc == DS_SUCCESS)
        rc = ds_rpn_calculator_pop(alloc, calc, out_res);

    ds_free(alloc, buf);
    return rc;
}
