<<<<<<< HEAD
#include "../include/ds/rpn_calculator.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

<<<<<<< HEAD:tests/test_rpn_calculator.c
int main(void) {
    printf("=== RPN Calculator ADT Unit Test Start ===\n");
=======
#include "data_structures.h"
 #include <stdio.h>
 #include <assert.h>
 #include <string.h>
>>>>>>> feature:tests/ds/test_rpn_calculator.c

    // 1. 電卓インスタンス生成
    ds_rpn_calculator_t* calc = ds_rpn_calculator_create();
    assert(calc != NULL);

    // 2. 基本的な足し算
    double result = 0.0;
    assert(ds_rpn_calculator_evaluate(calc, "3 4 +", &result) == DS_SUCCESS);
    assert(result == 7.0);

    // 3. 複数演算子・優先度のテスト (3 + 4 * 2 = 11)
    assert(ds_rpn_calculator_evaluate(calc, "3 4 2 * +", &result) == DS_SUCCESS);
    assert(result == 11.0);

    // 4. 括弧つき数式 ((3 + 4) * 2 = 14)
    assert(ds_rpn_calculator_evaluate(calc, "3 4 + 2 *", &result) == DS_SUCCESS);
    assert(result == 14.0);

    // 5. 引き算・割り算 (10 - 2 / 4 = 9.5)
    assert(ds_rpn_calculator_evaluate(calc, "10 2 4 / -", &result) == DS_SUCCESS);
    assert(result == 9.5);

    // 6. 負の数やゼロ割りエラー
    assert(ds_rpn_calculator_evaluate(calc, "5 0 /", &result) == DS_ERR_INVALID_ARG);

    // 7. 入力異常（空文字列、無効な文字列）
    assert(ds_rpn_calculator_evaluate(calc, "", &result) == DS_ERR_INVALID_ARG);
    assert(ds_rpn_calculator_evaluate(calc, "3 4 x", &result) == DS_ERR_INVALID_ARG);

    // 8. 大きな数式の連続計算
    assert(ds_rpn_calculator_evaluate(calc, "1 2 + 3 4 + *", &result) == DS_SUCCESS); // (1+2)*(3+4)=21
    assert(result == 21.0);

    // 9. リセット操作の検証（複数回使えるか）
    assert(ds_rpn_calculator_reset(calc) == DS_SUCCESS);
    assert(ds_rpn_calculator_evaluate(calc, "6 2 /", &result) == DS_SUCCESS);
    assert(result == 3.0);

    // 10. NULLポインタ等の異常系
    assert(ds_rpn_calculator_evaluate(NULL, "3 4 +", &result) == DS_ERR_NULL_POINTER);
    assert(ds_rpn_calculator_evaluate(calc, NULL, &result) == DS_ERR_NULL_POINTER);
    assert(ds_rpn_calculator_evaluate(calc, "3 4 +", NULL) == DS_ERR_NULL_POINTER);

    // 11. 破棄処理のテスト
    assert(ds_rpn_calculator_destroy(calc) == DS_SUCCESS);

    printf("=== RPN Calculator ADT Unit Test Passed ===\n");
    return 0;
=======
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
>>>>>>> feature
}