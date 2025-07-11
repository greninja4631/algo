#include "ds/process_test.h" // 本体が必要なのでinclude
#include "ds/process.h"
#include "test_process.h"
#include <assert.h>

// 最小限のアサートマクロ
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { printf("[FAIL] %s\n", msg); assert(0); } \
        else         { printf("[PASS] %s\n", msg); } \
    } while (0)

void test__process_basic(void)
{
    ds_process_t *proc = NULL;
    ds_error_t err = ds_process_create(123, 45, &proc);
    DS_TEST_ASSERT(err == DS_SUCCESS, "create: DS_SUCCESS");
    DS_TEST_ASSERT(proc != NULL, "create: not NULL");
    DS_TEST_ASSERT(proc->id == 123, "id matches");
    DS_TEST_ASSERT(proc->burst_time == 45, "burst_time matches");

    err = ds_process_destroy(proc);
    DS_TEST_ASSERT(err == DS_SUCCESS, "destroy: DS_SUCCESS");
}

void test__process_edge_cases(void)
{
    ds_error_t err = ds_process_destroy(NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "destroy: NULL pointer");

    ds_error_t err2 = ds_process_create(1, 2, NULL);
    DS_TEST_ASSERT(err2 == DS_ERR_NULL_POINTER, "create: NULL out_process");
}