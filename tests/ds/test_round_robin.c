#include <stdio.h>
#include <assert.h>
#include <stdlib.h>  
#include "ds/process.h"
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

    ds_round_robin_scheduler_destroy(g_test_alloc, sched);
}
