#include "ds/process.h"
#include <stdlib.h>
#include <string.h>

/* ─────────── 本体定義（ここだけ！） ─────────── */
struct ds_process {
    int id;
    int burst_time;
};

/* ─────────── 実装 ─────────── */
ds_error_t ds_process_create(int id, int burst_time,
                             ds_process_t **out_process)
{
    if (!out_process) return DS_ERR_NULL_POINTER;
    ds_process_t *p = malloc(sizeof *p);
    if (!p) return DS_ERR_ALLOC;

    p->id         = id;
    p->burst_time = burst_time;
    *out_process  = p;
    return DS_SUCCESS;
}

ds_process_t *ds_process_clone(const ds_process_t *src)
{
    if (!src) return NULL;
    ds_process_t *copy = malloc(sizeof *copy);
    if (!copy) return NULL;
    memcpy(copy, src, sizeof *copy);
    return copy;
}

void ds_process_destroy(ds_process_t *p)
{
    free(p);
}

/* ─────────── ゲッター ─────────── */
int ds_process_get_id(const ds_process_t *p)          { return p ? p->id         : 0; }
int ds_process_get_burst_time(const ds_process_t *p)  { return p ? p->burst_time : 0; }