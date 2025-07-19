#include <stddef.h>
#include <stdlib.h>                 /* calloc / free */
#include "ds/process.h"
#include "util/logger.h"

/* 標準 malloc/free → 抽象アロケータ */
static void* _std_alloc(size_t n, size_t sz) { return calloc(n, sz); }
static void  _std_free (void* p)             { free(p);             }
static const ds_allocator_t g_alloc = { .alloc = _std_alloc, .free = _std_free };
#define G_ALLOC (&g_alloc)

#define TASSERT(c,m) do { if(c) ds_log(DS_LOG_LEVEL_INFO,"[PASS] %s",m); \
                           else ds_log(DS_LOG_LEVEL_ERROR,"[FAIL] %s",m);} while(0)

/* 基本動作 */
void test__process_basic(void)
{
    ds_process_t *proc = NULL;
    TASSERT(ds_process_create(G_ALLOC, 123, 45, &proc) == DS_SUCCESS, "create");
    TASSERT(proc,                                         "proc != NULL");
    TASSERT(ds_process_get_id(proc)        == 123,        "id getter");
    TASSERT(ds_process_get_burst_time(proc)== 45,         "burst getter");
    TASSERT(ds_process_destroy(G_ALLOC, proc) == DS_SUCCESS, "destroy");
}

/* 異常系 */
void test__process_edge_cases(void)
{
    /* NULL out ptr */
    TASSERT(ds_process_create(G_ALLOC, 1, 2, NULL) == DS_ERR_NULL_POINTER,
            "create NULL out");
    /* NULL destroy は no-op */
    TASSERT(ds_process_destroy(G_ALLOC, NULL) == DS_SUCCESS,
            "destroy NULL");
}
