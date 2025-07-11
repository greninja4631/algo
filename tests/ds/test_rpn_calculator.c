/**
 * @file test_rpn_calculator.c
 * @brief ds_rpn_calculator_t モジュールの単体テスト
 * @details 本ファイルは test_main.c から呼び出される。main関数は持たない。
 */

#include "ds/rpn_calculator.h"
#include "util/test_util.h"
#include "util/logger.h"
#include <math.h>    // fabs
#include <float.h>   // DBL_EPSILON

// プロジェクト共通アサーションマクロ
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

// 浮動小数誤差許容付き比較
#define DS_TEST_ASSERT_FLOAT_EQ(expected, actual, eps, msg) \
    DS_TEST_ASSERT(fabs((expected) - (actual)) < (eps), msg)

/**
 * @brief RPN電卓 基本テスト
 */
void ds_test_rpn_calculator_basic(void)
{
    ds_error_t err;
    ds_rpn_calculator_t* calc = NULL;
    double result = 0.0;

    // --- 生成 ---
    err = ds_rpn_calculator_create(&calc);
    DS_TEST_ASSERT(err == DS_SUCCESS, "create: DS_SUCCESS");
    DS_TEST_ASSERT(calc != NULL, "create: not NULL");

    // --- Push/Popテスト ---
    err = ds_rpn_calculator_push(calc, 5.0);
    DS_TEST_ASSERT(err == DS_SUCCESS, "push 5.0");
    err = ds_rpn_calculator_push(calc, 7.0);
    DS_TEST_ASSERT(err == DS_SUCCESS, "push 7.0");
    double popped = 0.0;
    err = ds_rpn_calculator_pop(calc, &popped);
    DS_TEST_ASSERT(err == DS_SUCCESS && fabs(popped - 7.0) < DBL_EPSILON, "pop == 7.0");
    err = ds_rpn_calculator_pop(calc, &popped);
    DS_TEST_ASSERT(err == DS_SUCCESS && fabs(popped - 5.0) < DBL_EPSILON, "pop == 5.0");

    // --- Stack初期化 ---
    err = ds_rpn_calculator_reset(calc);
    DS_TEST_ASSERT(err == DS_SUCCESS, "reset: DS_SUCCESS");

    // --- 式評価（加算）---
    err = ds_rpn_calculator_evaluate(calc, "3 4 +", &result);
    DS_TEST_ASSERT(err == DS_SUCCESS, "evaluate: 3 4 +");
    DS_TEST_ASSERT_FLOAT_EQ(7.0, result, DBL_EPSILON, "evaluate: 3 4 + == 7.0");

    // --- 式評価（乗算）---
    err = ds_rpn_calculator_evaluate(calc, "2 3 *", &result);
    DS_TEST_ASSERT(err == DS_SUCCESS, "evaluate: 2 3 *");
    DS_TEST_ASSERT_FLOAT_EQ(6.0, result, DBL_EPSILON, "evaluate: 2 3 * == 6.0");

    // --- 式評価（複合）---
    err = ds_rpn_calculator_evaluate(calc, "5 1 2 + 4 * + 3 -", &result);
    DS_TEST_ASSERT(err == DS_SUCCESS, "evaluate: 5 1 2 + 4 * + 3 -");
    DS_TEST_ASSERT_FLOAT_EQ(14.0, result, DBL_EPSILON, "evaluate: 5 1 2 + 4 * + 3 - == 14.0");

    // --- 異常系 ---
    err = ds_rpn_calculator_evaluate(calc, "1 2", &result); // 式不完全
    DS_TEST_ASSERT(err != DS_SUCCESS, "evaluate: invalid (stack残り)");

    // --- 破棄 ---
    err = ds_rpn_calculator_destroy(calc);
    DS_TEST_ASSERT(err == DS_SUCCESS, "destroy: DS_SUCCESS");
    DS_TEST_ASSERT(ds_rpn_calculator_destroy(NULL) == DS_ERR_NULL_POINTER, "destroy: NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_rpn_calculator_basic 完了");
}

/**
 * @brief NULL安全・エラー系
 */
void ds_test_rpn_calculator_edge_cases(void)
{
    ds_error_t err;
    ds_rpn_calculator_t* calc = NULL;
    double result = 0.0;
    double out = 0.0;

    err = ds_rpn_calculator_push(NULL, 1.0);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "push: NULL");
    err = ds_rpn_calculator_pop(NULL, &out);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "pop: NULL");
    err = ds_rpn_calculator_pop(calc, NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "pop: out=NULL");
    err = ds_rpn_calculator_reset(NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "reset: NULL");
    err = ds_rpn_calculator_evaluate(NULL, "1 2 +", &result);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "evaluate: NULL calc");
    err = ds_rpn_calculator_evaluate(calc, NULL, &result);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "evaluate: NULL expr");
    err = ds_rpn_calculator_evaluate(calc, "1 2 +", NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "evaluate: NULL result");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_rpn_calculator_edge_cases 完了");
}