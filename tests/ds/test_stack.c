/**
 * @file test_stack.c
 * @brief ds_stack_t モジュールの単体テスト
 * @details 本ファイルは test_main.c から呼び出される。main関数は持たない。
 */

#include "ds/stack.h"
#include "util/test_util.h"
#include "util/logger.h"
#include <string.h> // memset

// アサーションマクロ（CI/CD対応ログ統一）
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

/**
 * @brief スタックの基本操作テスト
 */
void ds_test_stack_basic(void)
{
    ds_error_t err;
    ds_stack_t* stack = NULL;
    int a = 10, b = 20, c = 30;
    void* out = NULL;

    // --- 生成 ---
    err = ds_stack_create(&stack);
    DS_TEST_ASSERT(err == DS_SUCCESS, "create: DS_SUCCESS");
    DS_TEST_ASSERT(stack != NULL, "create: not NULL");
    DS_TEST_ASSERT(ds_stack_is_empty(stack), "is_empty after create");
    DS_TEST_ASSERT(ds_stack_size(stack) == 0, "size == 0 after create");

    // --- push ---
    err = ds_stack_push(stack, &a);
    DS_TEST_ASSERT(err == DS_SUCCESS, "push a");
    err = ds_stack_push(stack, &b);
    DS_TEST_ASSERT(err == DS_SUCCESS, "push b");
    err = ds_stack_push(stack, &c);
    DS_TEST_ASSERT(err == DS_SUCCESS, "push c");
    DS_TEST_ASSERT(ds_stack_size(stack) == 3, "size == 3 after push 3");

    // --- peek ---
    err = ds_stack_peek(stack, &out);
    DS_TEST_ASSERT(err == DS_SUCCESS && (*(int*)out) == c, "peek == c");

    // --- pop ---
    err = ds_stack_pop(stack, &out);
    DS_TEST_ASSERT(err == DS_SUCCESS && (*(int*)out) == c, "pop == c");
    err = ds_stack_pop(stack, &out);
    DS_TEST_ASSERT(err == DS_SUCCESS && (*(int*)out) == b, "pop == b");
    err = ds_stack_pop(stack, &out);
    DS_TEST_ASSERT(err == DS_SUCCESS && (*(int*)out) == a, "pop == a");
    DS_TEST_ASSERT(ds_stack_is_empty(stack), "is_empty after all pop");

    // --- pop空 ---
    err = ds_stack_pop(stack, &out);
    DS_TEST_ASSERT(err != DS_SUCCESS, "pop: empty");

    // --- 破棄 ---
    err = ds_stack_destroy(stack);
    DS_TEST_ASSERT(err == DS_SUCCESS, "destroy: DS_SUCCESS");
    DS_TEST_ASSERT(ds_stack_destroy(NULL) == DS_ERR_NULL_POINTER, "destroy: NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_stack_basic 完了");
}

/**
 * @brief スタックのNULL・異常系テスト
 */
void ds_test_stack_edge_cases(void)
{
    ds_error_t err;
    ds_stack_t* stack = NULL;
    void* out = NULL;

    // NULL操作
    err = ds_stack_push(NULL, &out);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "push: NULL");
    err = ds_stack_pop(NULL, &out);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "pop: NULL");
    err = ds_stack_peek(NULL, &out);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "peek: NULL");
    err = ds_stack_is_empty(NULL);
    DS_TEST_ASSERT(err, "is_empty: NULL returns true");
    err = ds_stack_size(NULL);
    DS_TEST_ASSERT(err == 0, "size: NULL returns 0");
    err = ds_stack_reset(NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "reset: NULL");
    err = ds_stack_get_stats(NULL, NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "get_stats: NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_stack_edge_cases 完了");
}

/**
 * @brief メモリ管理・リセット・統計テスト
 */
void ds_test_stack_memory_management(void)
{
    ds_error_t err;
    ds_stack_t* stack = NULL;
    int a = 100;
    ds_stats_t stats;
    void* out = NULL;

    err = ds_stack_create(&stack);
    DS_TEST_ASSERT(err == DS_SUCCESS, "create: DS_SUCCESS");
    err = ds_stack_push(stack, &a);
    DS_TEST_ASSERT(err == DS_SUCCESS, "push: a");
    err = ds_stack_reset(stack);
    DS_TEST_ASSERT(err == DS_SUCCESS, "reset: DS_SUCCESS");
    DS_TEST_ASSERT(ds_stack_is_empty(stack), "is_empty after reset");

    // 統計取得
    memset(&stats, 0, sizeof(stats));
    err = ds_stack_get_stats(stack, &stats);
    DS_TEST_ASSERT(err == DS_SUCCESS, "get_stats: DS_SUCCESS");
    // stats構造体の実装内容によって個別に確認（要プロジェクト実装参照）

    // 破棄
    err = ds_stack_destroy(stack);
    DS_TEST_ASSERT(err == DS_SUCCESS, "destroy: DS_SUCCESS");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_stack_memory_management 完了");
}