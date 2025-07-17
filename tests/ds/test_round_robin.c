<<<<<<< HEAD
<<<<<<< HEAD
#include "../include/ds/round_robin.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

<<<<<<< HEAD:tests/test_round_robin.c
int main(void) {
    printf("=== Round Robin Scheduler ADT Unit Test Start ===\n");
=======
#include "data_structures.h"
 #include <stdio.h>
 #include <assert.h>
 #include <string.h>
>>>>>>> feature:tests/ds/test_round_robin.c

    ds_round_robin_scheduler_t* scheduler = ds_round_robin_scheduler_create(10);
    assert(scheduler != NULL);

    ds_process_t p1 = {1, "proc-A", 5, 30, 1000};
    ds_process_t p2 = {2, "proc-B", 8, 20, 2000};
    ds_process_t p3 = {3, "proc-C", 3, 15, 3000};

    assert(ds_round_robin_scheduler_add_process(scheduler, &p1) == DS_SUCCESS);
    assert(ds_round_robin_scheduler_add_process(scheduler, &p2) == DS_SUCCESS);
    assert(ds_round_robin_scheduler_add_process(scheduler, &p3) == DS_SUCCESS);

    ds_process_t out;
    assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_SUCCESS);
    assert(out.process_id == 1);

    assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_SUCCESS);
    assert(out.process_id == 2);

    assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_SUCCESS);
    assert(out.process_id == 3);

    assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_SUCCESS);
    assert(out.process_id == 1);

    assert(ds_round_robin_scheduler_complete_process(scheduler, 2) == DS_SUCCESS);

    assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_SUCCESS);
    assert(out.process_id != 2);

    // --- ここから異常系を全て ERR で
    assert(ds_round_robin_scheduler_add_process(NULL, &p1) == DS_ERR_NULL_POINTER);
    assert(ds_round_robin_scheduler_add_process(scheduler, NULL) == DS_ERR_NULL_POINTER);
    assert(ds_round_robin_scheduler_complete_process(scheduler, 99) == DS_ERR_NOT_FOUND);
    assert(ds_round_robin_scheduler_get_next_process(NULL, &out) == DS_ERR_NULL_POINTER);
    assert(ds_round_robin_scheduler_get_next_process(scheduler, NULL) == DS_ERR_NULL_POINTER);

    assert(ds_round_robin_scheduler_complete_process(scheduler, 1) == DS_SUCCESS);
    assert(ds_round_robin_scheduler_complete_process(scheduler, 3) == DS_SUCCESS);

    assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_ERR_EMPTY_CONTAINER);

    assert(ds_round_robin_scheduler_destroy(scheduler) == DS_SUCCESS);

    printf("=== Round Robin Scheduler ADT Unit Test Passed ===\n");
    return 0;
=======
/**
 * @file test_round_robin.c
 * @brief ds_round_robin_scheduler_t モジュールの単体テスト
 * @details 本ファイルは test_main.c から呼び出す（main関数は持たない）。
 */

// 必須インクルード
#include "util/test_util.h"
=======
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>  
#include "ds/process.h"
>>>>>>> feature
#include "ds/round_robin.h"

// グローバルまたはローカルでテスト用アロケータを用意
static ds_allocator_t g_test_allocator = {
    .alloc = calloc,
    .free  = free
};
#define g_test_alloc (&g_test_allocator)

#define DS_TEST_ASSERT(cond, msg) \
    do { if (!(cond)) { printf("[FAIL] %s\n", msg); assert(0); } \
         else         { printf("[PASS] %s\n", msg); } } while (0)

static ds_process_t *make_process(int id, int burst) {
    ds_process_t *p = NULL;
    ds_error_t rc = ds_process_create(g_test_alloc, id, burst, &p);
    assert(rc == DS_SUCCESS && p != NULL);
    return p;
}

void test__round_robin_basic(void) {
    ds_round_robin_scheduler_t *sched = NULL;
    ds_error_t rc = ds_round_robin_scheduler_create(g_test_alloc, 10, &sched);
    assert(rc == DS_SUCCESS && sched != NULL);

    ds_error_t add_rc1 = ds_round_robin_scheduler_add_process(g_test_alloc, sched, make_process(1, 15));
    ds_error_t add_rc2 = ds_round_robin_scheduler_add_process(g_test_alloc, sched, make_process(2, 30));
    ds_error_t add_rc3 = ds_round_robin_scheduler_add_process(g_test_alloc, sched, make_process(3, 25));
    assert(add_rc1 == DS_SUCCESS && add_rc2 == DS_SUCCESS && add_rc3 == DS_SUCCESS);

    ds_process_t *out = NULL;
    ds_error_t get_rc1 = ds_round_robin_scheduler_get_next_process(g_test_alloc, sched, &out);
    DS_TEST_ASSERT(get_rc1 == DS_SUCCESS && ds_process_get_id(out) == 1, "id=1");
    ds_process_destroy(out);

    ds_error_t get_rc2 = ds_round_robin_scheduler_get_next_process(g_test_alloc, sched, &out);
    DS_TEST_ASSERT(get_rc2 == DS_SUCCESS && ds_process_get_id(out) == 2, "id=2");
    ds_process_destroy(out);

    ds_error_t get_rc3 = ds_round_robin_scheduler_get_next_process(g_test_alloc, sched, &out);
    DS_TEST_ASSERT(get_rc3 == DS_SUCCESS && ds_process_get_id(out) == 3, "id=3");
    ds_process_destroy(out);

<<<<<<< HEAD
<<<<<<< HEAD
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
>>>>>>> feature
=======
    ds_round_robin_scheduler_destroy(sched);
>>>>>>> feature
}
=======
    ds_round_robin_scheduler_destroy(g_test_alloc, sched);
}
>>>>>>> feature
