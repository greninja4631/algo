/**
 * @file rpn_calculator.c
 * @brief 逆ポーランド記法（RPN）電卓ADT実装
 * @version 1.0.0
 * @date 2025-05-28
 *
 * 【設計意図】
 * - 単一責務: スタックADTを活用しRPN式の評価のみ担当
 * - 情報隠蔽: 内部構造はAPI外から不透明
 * - エラー処理・メモリ安全・CI/CD/クラウド適合を重視
 * - 依存性注入（ログ/メモリ/スタック）でテスト性・運用性強化
 */

#include "include/data_structures.h"
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include <ctype.h>
 #include <math.h>
 
 /* --- RPN電卓構造体 --- */
 struct ds_rpn_calculator {
     ds_stack_t* stack;            // 計算用スタック
 };
 
 /* --- メモリ管理/ログ関数（依存性注入） --- */
 static ds_malloc_func_t ds_malloc = malloc;
 static ds_free_func_t ds_free = free;
 static ds_log_func_t ds_log = NULL;
 
 static void default_log(ds_log_level_t level, const char* fmt, ...) {
     (void)level;
     va_list args;
     va_start(args, fmt);
     vfprintf(stderr, fmt, args);
     va_end(args);
 }
 
 /* --- ユーティリティ：トークンが数値か判定 --- */
 static bool is_number(const char* token, double* val) {
     if (!token || !*token) return false;
     char* endptr = NULL;
     double num = strtod(token, &endptr);
     if (endptr == token || *endptr != '\0') return false;
     if (val) *val = num;
     return true;
 }
 
 /* --- 公開API実装 --- */
 
 /**
  * @brief RPN電卓生成
  */
 ds_rpn_calculator_t* ds_rpn_calculator_create(void) {
     ds_rpn_calculator_t* calc = (ds_rpn_calculator_t*)ds_malloc(sizeof(ds_rpn_calculator_t));
     if (!calc) {
         if (ds_log) ds_log(DS_LOG_ERROR, "[ds_rpn_calculator_create] メモリ確保失敗\n");
         return NULL;
     }
     calc->stack = ds_stack_create();
     if (!calc->stack) {
         ds_free(calc);
         return NULL;
     }
     return calc;
 }
 
 /**
  * @brief RPN電卓破棄
  */
 ds_error_t ds_rpn_calculator_destroy(ds_rpn_calculator_t* calc) {
     if (!calc) return DS_ERR_NULL_POINTER;
     ds_stack_destroy(calc->stack);
     ds_free(calc);
     return DS_SUCCESS;
 }
 
 /**
  * @brief RPN式を評価
  */
 ds_error_t ds_rpn_calculator_evaluate(ds_rpn_calculator_t* calc, const char* expression, double* result) {
     if (!calc || !expression || !result) return DS_ERR_NULL_POINTER;
     ds_stack_t* stack = calc->stack;
     ds_stack_reset(stack);  // スタック状態リセット（実装されていれば）
     char* expr = strdup(expression);  // strtok破壊対策
     if (!expr) return DS_ERR_ALLOC;
     char* token = strtok(expr, " ");
     ds_error_t err = DS_SUCCESS;
     while (token) {
         double val;
         if (is_number(token, &val)) {
             double* pdata = (double*)ds_malloc(sizeof(double));
             if (!pdata) { err = DS_ERR_ALLOC; break; }
             *pdata = val;
             if ((err = ds_stack_push(stack, pdata)) != DS_SUCCESS) break;
         } else if (strlen(token) == 1 && strchr("+-*/", token[0])) {
             void* op2ptr; void* op1ptr;
             if ((err = ds_stack_pop(stack, &op2ptr)) != DS_SUCCESS) break;
             if ((err = ds_stack_pop(stack, &op1ptr)) != DS_SUCCESS) { ds_free(op2ptr); break; }
             double op2 = *(double*)op2ptr;
             double op1 = *(double*)op1ptr;
             ds_free(op2ptr); ds_free(op1ptr);
             double res = 0;
             switch(token[0]) {
                 case '+': res = op1 + op2; break;
                 case '-': res = op1 - op2; break;
                 case '*': res = op1 * op2; break;
                 case '/': if (op2 == 0) { err = DS_ERR_INVALID_ARG; break; }
                           res = op1 / op2; break;
             }
             if (err != DS_SUCCESS) break;
             double* pdata = (double*)ds_malloc(sizeof(double));
             if (!pdata) { err = DS_ERR_ALLOC; break; }
             *pdata = res;
             if ((err = ds_stack_push(stack, pdata)) != DS_SUCCESS) break;
         } else {
             err = DS_ERR_INVALID_ARG;
             break;
         }
         token = strtok(NULL, " ");
     }
     // 結果取得
     if (err == DS_SUCCESS) {
         void* rptr;
         if ((err = ds_stack_pop(stack, &rptr)) == DS_SUCCESS) {
             *result = *(double*)rptr;
             ds_free(rptr);
         }
     }
     free(expr);
     return err;
 }
 
 /**
  * @brief スタック状態クリア（全要素破棄）
  */
 ds_error_t ds_rpn_calculator_reset(ds_rpn_calculator_t* calc) {
     if (!calc) return DS_ERR_NULL_POINTER;
     void* data;
     while (ds_stack_pop(calc->stack, &data) == DS_SUCCESS) {
         ds_free(data);
     }
     return DS_SUCCESS;
 }
 
 /**
  * @brief ログ・メモリ関数差し替え
  */
 void ds_set_log_function(ds_log_func_t func) { ds_log = func ? func : default_log; }
 void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func) {
     ds_malloc = malloc_func ? malloc_func : malloc;
     ds_free = free_func ? free_func : free;
 }