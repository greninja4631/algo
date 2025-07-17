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

/* ───── 内部構造体 (Opaque 実体) ───── */
struct ds_rpn_calculator {
    const ds_allocator_t *alloc;   /* DI アロケータを保持 */
    ds_stack_t           *stack;   /* オペランドスタック */
};

/* ───── 内部ユーティリティ ───── */
static int
is_number(const char *s, double *out_val)
{
    char *end = NULL;
    *out_val  = strtod(s, &end);
    return (end != s && *end == '\0');
}

/*───────────────────────────────────────────────
 *                  API 実装
 *──────────────────────────────────────────────*/

/* 生成 */
ds_error_t
ds_rpn_calculator_create(const ds_allocator_t *alloc,
                         ds_rpn_calculator_t **out_calc)
{
    if (!alloc || !out_calc) return DS_ERR_NULL_POINTER;

    ds_rpn_calculator_t *calc =
        (ds_rpn_calculator_t *)ds_malloc(alloc, 1, sizeof(ds_rpn_calculator_t));
    if (!calc) return DS_ERR_ALLOC;

    calc->alloc = alloc;
    ds_error_t rc = ds_stack_create(alloc, &calc->stack);
    if (rc != DS_SUCCESS) { ds_free(alloc, calc); return rc; }

    *out_calc = calc;
    return DS_SUCCESS;
}

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
    if (!calc) return DS_ERR_NULL_POINTER;
    return ds_stack_reset(calc->alloc, calc->stack);
}

/* プッシュ */
ds_error_t
ds_rpn_calculator_push(ds_rpn_calculator_t *calc, double v)
{
    if (!calc) return DS_ERR_NULL_POINTER;

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
            rc = DS_ERR_INVALID_ARG;
        }

        if (rc != DS_SUCCESS) break;
    }

    if (rc == DS_SUCCESS) {
        rc = ds_rpn_calculator_pop(calc, out_res);
    }

    ds_free(calc->alloc, buf);
    return rc;
}
