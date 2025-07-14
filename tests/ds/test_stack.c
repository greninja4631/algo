/**
 * @file  tests/ds/test_stack.c
 * @brief ds_stack_t モジュールの単体テスト
 * @note  main() は tests/test_main.c に集約される
 */
#include "ds/stack.h"
#include "util/test_util.h"
#include "util/logger.h"
#include <string.h>   /* memset */

/* ───────────── 共通アサート ───────────── */
#define DS_TEST_ASSERT(cond, msg)                                          \
    do {                                                                   \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO,  "[PASS] %s", msg); }        \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",        \
                           msg, __FILE__, __LINE__);                       \
        }                                                                  \
    } while (0)

/* ────────────────────────────────────── */
/* 基本操作テスト                                                              */
void ds_test_stack_basic(void)
{
    ds_stack_t *stack = NULL;
    int a = 10, b = 20, c = 30;
    void *out = NULL;

    DS_TEST_ASSERT(ds_stack_create(&stack) == DS_SUCCESS && stack,
                   "create");

    ds_stack_push(stack, &a);
    ds_stack_push(stack, &b);
    ds_stack_push(stack, &c);
    DS_TEST_ASSERT(ds_stack_size(stack) == 3, "size == 3");

    ds_stack_peek(stack, &out);
    DS_TEST_ASSERT(*(int *)out == c, "peek == c");

    ds_stack_pop(stack, &out); DS_TEST_ASSERT(*(int *)out == c, "pop c");
    ds_stack_pop(stack, &out); DS_TEST_ASSERT(*(int *)out == b, "pop b");
    ds_stack_pop(stack, &out); DS_TEST_ASSERT(*(int *)out == a, "pop a");

    DS_TEST_ASSERT(ds_stack_is_empty(stack), "empty after pop");
    ds_stack_destroy(stack);
}

/* 異常系テスト */
void ds_test_stack_edge_cases(void)
{
    void *dummy = NULL;

    DS_TEST_ASSERT(ds_stack_push(NULL, &dummy) == DS_ERR_NULL_POINTER,
                   "push NULL");
    DS_TEST_ASSERT(ds_stack_pop(NULL, &dummy)  == DS_ERR_NULL_POINTER,
                   "pop NULL");
    DS_TEST_ASSERT(ds_stack_peek(NULL, &dummy) == DS_ERR_NULL_POINTER,
                   "peek NULL");

    /* 戻り値は使わないが、呼び出しがクラッシュしないことを確認 */
    (void)ds_stack_is_empty(NULL);
    (void)ds_stack_size(NULL);
}

/* メモリ / 統計テスト */
void ds_test_stack_memory_management(void)
{
    ds_stack_t *stack = NULL;
    ds_stats_t  stats;
    int val = 100;

    ds_stack_create(&stack);
    ds_stack_push(stack, &val);
    ds_stack_reset(stack);

    memset(&stats, 0, sizeof stats);
    ds_stack_get_stats(stack, &stats);   /* 値はプロジェクト実装に依存 */

    ds_stack_destroy(stack);
}