<<<<<<< HEAD
<<<<<<< HEAD
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
=======
=======
#include <stddef.h>
>>>>>>> feature
/*
 * tests/ds/test_round_robin.c
 * Round-Robin Scheduler – unit tests (2025-07 guideline)
 */
#include <stdlib.h>
>>>>>>> feature
#include <assert.h>
#include "ds/process.h"
>>>>>>> feature
#include "ds/round_robin.h"
#include "util/logger.h"

/* ── 1. アロケータ DI ── */
static void* _std_alloc(size_t c,size_t z){ return c?calloc(c,z):NULL; }
static void  _std_free (void* p){ if(p) free(p); }
static const ds_allocator_t G_ALLOC_IMPL = { .alloc=_std_alloc, .free=_std_free };
#define G_ALLOC (&G_ALLOC_IMPL)

/* ── 2. ASSERT ── */
#define TASSERT(x,msg) do{ if(x) ds_log(DS_LOG_LEVEL_INFO ,"[PASS] %s",msg); \
                           else { ds_log(DS_LOG_LEVEL_ERROR,"[FAIL] %s",msg); assert(0);} }while(0)

/* ── 3. helper ── */
static ds_process_t *mk_proc(int32_t id,int32_t bt){
    ds_process_t *p=NULL;
    TASSERT(ds_process_create(G_ALLOC,id,bt,&p)==DS_SUCCESS&&p,"proc create");
    return p;
}

/* ── 4-A. 基本テスト ── */
void test__round_robin_basic(void)
{
    const int32_t QUANTUM = 4;
    ds_round_robin_scheduler_t *sch=NULL;
    TASSERT(ds_round_robin_scheduler_create(G_ALLOC,QUANTUM,&sch)==DS_SUCCESS,"create");

    TASSERT(ds_round_robin_scheduler_add_process(G_ALLOC,sch,mk_proc(1,15))==DS_SUCCESS,"add p1");
    TASSERT(ds_round_robin_scheduler_add_process(G_ALLOC,sch,mk_proc(2,30))==DS_SUCCESS,"add p2");
    TASSERT(ds_round_robin_scheduler_add_process(G_ALLOC,sch,mk_proc(3,25))==DS_SUCCESS,"add p3");

#ifdef DS_RR_HAS_SIZE_API
    TASSERT(ds_round_robin_scheduler_size(sch)==3,"size 3");
#endif

    ds_process_t *p=NULL;
    TASSERT(ds_round_robin_scheduler_get_next_process(G_ALLOC,sch,&p)==DS_SUCCESS&&ds_process_get_id(p)==1,"pop 1");
    ds_process_destroy(G_ALLOC,p);

    TASSERT(ds_round_robin_scheduler_get_next_process(G_ALLOC,sch,&p)==DS_SUCCESS&&ds_process_get_id(p)==2,"pop 2");
    ds_process_destroy(G_ALLOC,p);

<<<<<<< HEAD
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
=======
    TASSERT(ds_round_robin_scheduler_get_next_process(G_ALLOC,sch,&p)==DS_SUCCESS&&ds_process_get_id(p)==3,"pop 3");
    ds_process_destroy(G_ALLOC,p);

#ifdef DS_RR_HAS_SIZE_API
    TASSERT(ds_round_robin_scheduler_size(sch)==0,"size 0");
#endif
    ds_round_robin_scheduler_destroy(G_ALLOC,sch);
}

/* ── 4-B. 異常系 ── */
void test__round_robin_edge_cases(void)
{
    ds_process_t *dummy=NULL;
    TASSERT(ds_round_robin_scheduler_add_process(G_ALLOC,NULL,mk_proc(9,9))==DS_ERR_NULL_POINTER,"add NULL");
    TASSERT(ds_round_robin_scheduler_get_next_process(G_ALLOC,NULL,&dummy)==DS_ERR_NULL_POINTER,"get NULL");
}

/* optional stand-alone */
#ifdef DS_STANDALONE_TEST
int main(void){ test__round_robin_basic(); test__round_robin_edge_cases(); return 0; }
#endif
>>>>>>> feature
