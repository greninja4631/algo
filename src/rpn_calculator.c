#include "../include/data_structures.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>

// ---- 依存性注入メモリ/ログ関数（.cファイル内で本当に必要な場合のみstaticで宣言！） ----
static ds_malloc_func_t ds_malloc = malloc;
static ds_free_func_t ds_free = free;
static ds_log_func_t ds_log = NULL;

// デフォルトログ関数（外部から切り替え可能）
static void default_log(ds_log_level_t level, const char* fmt, ...) {
    (void)level;
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

// 外部から依存性注入できるAPI
void ds_set_log_function(ds_log_func_t func) { ds_log = func ? func : default_log; }
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func) {
    ds_malloc = malloc_func ? malloc_func : malloc;
    ds_free = free_func ? free_func : free;
}

// ---- 構造体本体（本当に必要なメンバだけ） ----
struct ds_rpn_calculator {
    ds_stack_t* stack;  // スタックは別途実装
};

// ---- 補助関数 ----
static int is_number(const char* str, double* out_val) {
    char* endptr = NULL;
    *out_val = strtod(str, &endptr);
    return (endptr != str && *endptr == '\0');
}

// ---- RPN電卓API本体 ----

ds_rpn_calculator_t* ds_rpn_calculator_create(void) {
    ds_rpn_calculator_t* calc = (ds_rpn_calculator_t*)ds_malloc(sizeof(ds_rpn_calculator_t));
    if (!calc) return NULL;
    calc->stack = ds_stack_create();
    if (!calc->stack) {
        ds_free(calc);
        return NULL;
    }
    return calc;
}

ds_error_t ds_rpn_calculator_destroy(ds_rpn_calculator_t* calc) {
    if (!calc) return DS_ERR_NULL_POINTER;
    ds_stack_destroy(calc->stack);
    ds_free(calc);
    return DS_SUCCESS;
}

ds_error_t ds_rpn_calculator_reset(ds_rpn_calculator_t* calc) {
    if (!calc) return DS_ERR_NULL_POINTER;
    return ds_stack_reset(calc->stack);
}

ds_error_t ds_rpn_calculator_push(ds_rpn_calculator_t* calc, double value) {
    if (!calc) return DS_ERR_NULL_POINTER;
    // 値をheapに保存（スタックAPIがvoid*を要求する前提）
    double* val_ptr = (double*)ds_malloc(sizeof(double));
    if (!val_ptr) return DS_ERR_ALLOC;
    *val_ptr = value;
    return ds_stack_push(calc->stack, val_ptr);
}

ds_error_t ds_rpn_calculator_pop(ds_rpn_calculator_t* calc, double* out) {
    if (!calc || !out) return DS_ERR_NULL_POINTER;
    void* ptr = NULL;
    ds_error_t err = ds_stack_pop(calc->stack, &ptr);
    if (err != DS_SUCCESS) return err;
    if (!ptr) return DS_ERR_EMPTY;
    *out = *(double*)ptr;
    ds_free(ptr);
    return DS_SUCCESS;
}

// --- RPN式評価 ---
ds_error_t ds_rpn_calculator_evaluate(ds_rpn_calculator_t* calc, const char* expression, double* result) {
    if (!calc || !expression || !result) return DS_ERR_INVALID_ARG;
    ds_error_t err = DS_SUCCESS;
    ds_rpn_calculator_reset(calc);

    char* expr_copy = strdup(expression);
    if (!expr_copy) return DS_ERR_ALLOC;

    char* token = strtok(expr_copy, " ");
    while (token) {
        double value;
        if (is_number(token, &value)) {
            if ((err = ds_rpn_calculator_push(calc, value)) != DS_SUCCESS) break;
        } else if (strlen(token) == 1 && strchr("+-*/", token[0])) {
            double rhs, lhs;
            if ((err = ds_rpn_calculator_pop(calc, &rhs)) != DS_SUCCESS) break;
            if ((err = ds_rpn_calculator_pop(calc, &lhs)) != DS_SUCCESS) break;

            double result_val = 0;
            switch (token[0]) {
                case '+': result_val = lhs + rhs; break;
                case '-': result_val = lhs - rhs; break;
                case '*': result_val = lhs * rhs; break;
                case '/':
                    if (rhs == 0) { err = DS_ERR_INVALID_ARG; break; }
                    result_val = lhs / rhs;
                    break;
                default: err = DS_ERR_INVALID_ARG; break;
            }
            if (err != DS_SUCCESS) break;
            if ((err = ds_rpn_calculator_push(calc, result_val)) != DS_SUCCESS) break;
        } else {
            err = DS_ERR_INVALID_ARG;
            break;
        }
        token = strtok(NULL, " ");
    }

    if (err == DS_SUCCESS) {
        double final;
        if ((err = ds_rpn_calculator_pop(calc, &final)) == DS_SUCCESS) {
            *result = final;
        }
    }
    free(expr_copy);
    return err;
}