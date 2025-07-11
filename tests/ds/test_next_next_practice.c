#include "ds/next_next_practice.h"      // ← API宣言
#include "util/logger.h"                // ← ロガー
#include "ds/test_next_next_practice.h" // ← テスト宣言（パス短縮！）
// プロジェクト共通アサーションマクロ
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

// 基本動作テスト
void test__next_next_practice_basic(void)
{
    ds_next_next_practice_t* obj = ds_next_next_practice_create();
    DS_TEST_ASSERT(obj != NULL, "create: not NULL");

    // テスト/デモAPI実行
    ds_error_t err;

    err = demonstrate_array_operations();
    DS_TEST_ASSERT(err == DS_SUCCESS, "demonstrate_array_operations");

    err = test_stack_operations();
    DS_TEST_ASSERT(err == DS_SUCCESS, "test_stack_operations");

    err = test_queue_operations();
    DS_TEST_ASSERT(err == DS_SUCCESS, "test_queue_operations");

    err = test_undo_redo_system();
    DS_TEST_ASSERT(err == DS_SUCCESS, "test_undo_redo_system");

    err = test_circular_and_round_robin();
    DS_TEST_ASSERT(err == DS_SUCCESS, "test_circular_and_round_robin");

    ds_next_next_practice_destroy(obj);
    DS_TEST_ASSERT(1, "destroy: no crash");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__next_next_practice_basic 完了");
}

// NULL安全・異常系など追加テスト例
void test__next_next_practice_edge_cases(void)
{
    ds_next_next_practice_destroy(NULL);
    DS_TEST_ASSERT(1, "destroy: NULL安全（クラッシュしない）");

    // 必要ならAPIのNULL・異常系もここに追加

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__next_next_practice_edge_cases 完了");
}