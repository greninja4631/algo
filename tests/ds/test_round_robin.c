/**
 * @file test_round_robin.c
 * @brief ds_round_robin_scheduler_t モジュールの単体テスト
 * @details 本ファイルは test_main.c から呼び出す（main関数は持たない）。
 */

// 必須インクルード
#include "util/test_util.h"
#include "ds/round_robin.h"
#include "ds/process_test.h"
#include "util/logger.h"
#include <stdlib.h>

// アサーションマクロ
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

// テスト用：プロセス生成API (APIが未実装の場合、テスト用に簡易で用意)
static ds_process_t* make_process(int id, int burst_time)
{
    ds_process_t* proc = (ds_process_t*)malloc(sizeof(ds_process_t));
    if (!proc) return NULL;
    proc->id = id;
    proc->burst_time = burst_time;
    return proc;
}

// 生成したプロセスを安全に解放
static void free_process(ds_process_t* proc)
{
    if (proc) free(proc);
}

/**
 * @brief ラウンドロビン基本動作テスト
 */
void test__round_robin_basic(void)
{
    ds_error_t err;
    ds_round_robin_scheduler_t* sched = NULL;
    ds_process_t* p1 = make_process(1, 15);
    ds_process_t* p2 = make_process(2, 30);
    ds_process_t* p3 = make_process(3, 25);
    ds_process_t* out_proc = NULL;

    // --- 生成 ---
    err = ds_round_robin_scheduler_create(10, &sched);
    DS_TEST_ASSERT(err == DS_SUCCESS, "create: DS_SUCCESS");
    DS_TEST_ASSERT(sched != NULL, "create: not NULL");

    // --- プロセス登録 ---
    err = ds_round_robin_scheduler_add_process(sched, p1);
    DS_TEST_ASSERT(err == DS_SUCCESS, "add_process: p1");
    err = ds_round_robin_scheduler_add_process(sched, p2);
    DS_TEST_ASSERT(err == DS_SUCCESS, "add_process: p2");
    err = ds_round_robin_scheduler_add_process(sched, p3);
    DS_TEST_ASSERT(err == DS_SUCCESS, "add_process: p3");

    // --- スケジューリング ---
    // 1回目
    err = ds_round_robin_scheduler_get_next_process(sched, &out_proc);
    DS_TEST_ASSERT(err == DS_SUCCESS && out_proc != NULL && out_proc->id == 1, "get_next_process: id=1");
    err = ds_round_robin_scheduler_complete_process(sched, 1);
    DS_TEST_ASSERT(err == DS_SUCCESS, "complete_process: id=1");

    // 2回目
    err = ds_round_robin_scheduler_get_next_process(sched, &out_proc);
    DS_TEST_ASSERT(err == DS_SUCCESS && out_proc != NULL && out_proc->id == 2, "get_next_process: id=2");
    err = ds_round_robin_scheduler_complete_process(sched, 2);
    DS_TEST_ASSERT(err == DS_SUCCESS, "complete_process: id=2");

    // 3回目
    err = ds_round_robin_scheduler_get_next_process(sched, &out_proc);
    DS_TEST_ASSERT(err == DS_SUCCESS && out_proc != NULL && out_proc->id == 3, "get_next_process: id=3");
    err = ds_round_robin_scheduler_complete_process(sched, 3);
    DS_TEST_ASSERT(err == DS_SUCCESS, "complete_process: id=3");

    // --- もうプロセスなし ---
    err = ds_round_robin_scheduler_get_next_process(sched, &out_proc);
    DS_TEST_ASSERT(err != DS_SUCCESS, "get_next_process: empty scheduler");

    // --- 破棄 ---
    err = ds_round_robin_scheduler_destroy(sched);
    DS_TEST_ASSERT(err == DS_SUCCESS, "destroy: DS_SUCCESS");
    DS_TEST_ASSERT(ds_round_robin_scheduler_destroy(NULL) == DS_ERR_NULL_POINTER, "destroy: NULL");

    // 生成プロセスの解放
    free_process(p1);
    free_process(p2);
    free_process(p3);

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__round_robin_basic 完了");
}

/**
 * @brief NULL安全・異常系テスト
 */
void test__round_robin_edge_cases(void)
{
    ds_error_t err;
    ds_process_t* dummy = make_process(99, 99);
    ds_process_t* out_proc = NULL;

    err = ds_round_robin_scheduler_create(0, NULL); // NULLポインタ
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "create: NULL out_sched");

    err = ds_round_robin_scheduler_add_process(NULL, dummy);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "add_process: NULL scheduler");

    err = ds_round_robin_scheduler_get_next_process(NULL, &out_proc);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "get_next_process: NULL scheduler");

    err = ds_round_robin_scheduler_get_next_process(NULL, NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "get_next_process: NULL both");

    err = ds_round_robin_scheduler_complete_process(NULL, 0);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "complete_process: NULL scheduler");

    err = ds_round_robin_scheduler_destroy(NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "destroy: NULL");

    free_process(dummy);

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__round_robin_edge_cases 完了");
}