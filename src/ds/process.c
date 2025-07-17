#include "ds/process.h"
#include "util/memory.h"   /* ds_malloc / ds_free */

struct ds_process {
    int32_t pid;
    int32_t burst;
};

ds_error_t ds_process_create(const ds_allocator_t *alloc,
                             int32_t               pid,
                             int32_t               burst,
                             ds_process_t        **out_proc)
{
    if (!alloc || !out_proc) return DS_ERR_NULL_POINTER;

    ds_process_t *p = ds_malloc(alloc, 1, sizeof *p);
    if (!p) return DS_ERR_ALLOC;

    p->pid   = pid;
    p->burst = burst;
    *out_proc = p;
    return DS_SUCCESS;
}

ds_error_t ds_process_destroy(const ds_allocator_t *alloc, ds_process_t *p)
{
    if (!alloc) return DS_ERR_NULL_POINTER;
    if (!p)     return DS_SUCCESS;
    ds_free(alloc, p);
    return DS_SUCCESS;
}

int32_t ds_process_get_id(const ds_process_t *p)         { return p ? p->pid   : -1; }
int32_t ds_process_get_burst_time(const ds_process_t *p) { return p ? p->burst : -1; }
