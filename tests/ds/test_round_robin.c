#include <stdio.h>
#include <assert.h>
#include "ds/process.h"
#include "ds/round_robin.h"

#define DS_TEST_ASSERT(cond, msg) \
    do { if (!(cond)) { printf("[FAIL] %s\n", msg); assert(0); } \
         else         { printf("[PASS] %s\n", msg); } } while (0)

static ds_process_t *make_process(int id, int burst)
{
    ds_process_t *p = NULL;
    ds_process_create(id, burst, &p);
    return p;
}

void test__round_robin_basic(void)
{
    ds_round_robin_scheduler_t *sched=NULL;
    ds_round_robin_scheduler_create(10,&sched);

    ds_round_robin_scheduler_add_process(sched, make_process(1,15));
    ds_round_robin_scheduler_add_process(sched, make_process(2,30));
    ds_round_robin_scheduler_add_process(sched, make_process(3,25));

    ds_process_t *out=NULL;
    ds_round_robin_scheduler_get_next_process(sched,&out);
    DS_TEST_ASSERT(ds_process_get_id(out)==1,"id=1");
    ds_process_destroy(out);

    ds_round_robin_scheduler_get_next_process(sched,&out);
    DS_TEST_ASSERT(ds_process_get_id(out)==2,"id=2");
    ds_process_destroy(out);

    ds_round_robin_scheduler_get_next_process(sched,&out);
    DS_TEST_ASSERT(ds_process_get_id(out)==3,"id=3");
    ds_process_destroy(out);

    ds_round_robin_scheduler_destroy(sched);
}