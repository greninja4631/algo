#include <stddef.h>
/*
 * tests/ds/test_round_robin.c
 * Round-Robin Scheduler – unit tests (2025-07 guideline)
 */
#include <stdlib.h>
#include <assert.h>
#include "ds/process.h"
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