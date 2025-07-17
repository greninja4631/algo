/**
 * @file    tests/ds/test_rpn_calculator.c
 * @brief   ds_rpn_calculator_t モジュールの単体テスト（2025-07ガイドライン対応）
 * @details 本ファイルは test_main.c から呼び出される。main関数は持たない。
 */

#include "ds/rpn_calculator.h"
#include "util/test_util.h"
#include "util/logger.h"
#include <math.h>
#include <float.h>

// ====== グローバル・アロケータ ======
extern const ds_allocator_t* g_alloc;

// ==== 本番printf禁止! ロガーラッパーのみ ====
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)
#define DS_TEST_ASSERT_FLOAT_EQ(expected, actual, eps, msg) \
    DS_TEST_ASSERT(fabs((expected) - (actual)) < (eps), msg)

/**
 * @brief RPN電卓 基本機能テスト
 * @details すべて「アロケータalloc経由」でAPI呼び出し。
 */
void test__rpn_calculator_basic(void)
{
    ds_error_t err;
    ds_rpn_calculator_t* calc = NULL;
    double result = 0.0;

    // --- 生成 ---
    err = ds_rpn_calculator_create(g_alloc, &calc);  // ←必ずallocを渡す
    DS_TEST_ASSERT(err == DS_SUCCESS, "create: DS_SUCCESS");
    DS_TEST_ASSERT(calc != NULL, "create: not NULL");

    // --- Push/Pop ---
    err = ds_rpn_calculator_push(g_alloc, calc, 5.0);
    DS_TEST_ASSERT(err == DS_SUCCESS, "push 5.0");
    err = ds_rpn_calculator_push(g_alloc, calc, 7.0);
    DS_TEST_ASSERT(err == DS_SUCCESS, "push 7.0");
    double popped = 0.0;
    err = ds_rpn_calculator_pop(g_alloc, calc, &popped);
    DS_TEST_ASSERT(err == DS_SUCCESS && fabs(popped - 7.0) < DBL_EPSILON, "pop == 7.0");
    err = ds_rpn_calculator_pop(g_alloc, calc, &popped);
    DS_TEST_ASSERT(err == DS_SUCCESS && fabs(popped - 5.0) < DBL_EPSILON, "pop == 5.0");

    // --- Stack初期化 ---
    err = ds_rpn_calculator_reset(g_alloc, calc);
    DS_TEST_ASSERT(err == DS_SUCCESS, "reset: DS_SUCCESS");

    // --- evaluate（加算/乗算/複合） ---
    err = ds_rpn_calculator_evaluate(g_alloc, calc, "3 4 +", &result);
    DS_TEST_ASSERT(err == DS_SUCCESS, "evaluate: 3 4 +");
    DS_TEST_ASSERT_FLOAT_EQ(7.0, result, DBL_EPSILON, "evaluate: 3 4 + == 7.0");

    err = ds_rpn_calculator_evaluate(g_alloc, calc, "2 3 *", &result);
    DS_TEST_ASSERT(err == DS_SUCCESS, "evaluate: 2 3 *");
    DS_TEST_ASSERT_FLOAT_EQ(6.0, result, DBL_EPSILON, "evaluate: 2 3 * == 6.0");

    err = ds_rpn_calculator_evaluate(g_alloc, calc, "5 1 2 + 4 * + 3 -", &result);
    DS_TEST_ASSERT(err == DS_SUCCESS, "evaluate: 5 1 2 + 4 * + 3 -");
    DS_TEST_ASSERT_FLOAT_EQ(14.0, result, DBL_EPSILON, "evaluate: ... == 14.0");

    // --- 異常系 ---
    err = ds_rpn_calculator_evaluate(g_alloc, calc, "1 2", &result); // 式不完全
    DS_TEST_ASSERT(err != DS_SUCCESS, "evaluate: invalid (stack残り)");

    // --- 破棄 ---
    err = ds_rpn_calculator_destroy(g_alloc, calc);   // @ownership: 呼び出し側で必ず解放
    DS_TEST_ASSERT(err == DS_SUCCESS, "destroy: DS_SUCCESS");
    DS_TEST_ASSERT(ds_rpn_calculator_destroy(g_alloc, NULL) == DS_ERR_NULL_POINTER, "destroy: NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__rpn_calculator_basic 完了");
}

/**
 * @brief NULL/異常系安全性テスト
 */
void test__rpn_calculator_edge_cases(void)
{
    ds_error_t err;
    ds_rpn_calculator_t* calc = NULL;
    double result = 0.0, out = 0.0;

    err = ds_rpn_calculator_push(g_alloc, NULL, 1.0);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "push: NULL");
    err = ds_rpn_calculator_pop(g_alloc, NULL, &out);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "pop: NULL");
    err = ds_rpn_calculator_pop(g_alloc, calc, NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "pop: out=NULL");
    err = ds_rpn_calculator_reset(g_alloc, NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "reset: NULL");
    err = ds_rpn_calculator_evaluate(g_alloc, NULL, "1 2 +", &result);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "evaluate: NULL calc");
    err = ds_rpn_calculator_evaluate(g_alloc, calc, NULL, &result);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "evaluate: NULL expr");
    err = ds_rpn_calculator_evaluate(g_alloc, calc, "1 2 +", NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "evaluate: NULL result");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__rpn_calculator_edge_cases 完了");
}