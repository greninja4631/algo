#include "ds/process.h"
#include <stdlib.h>  // malloc, free

// 本体定義（ここだけ）
struct ds_process {
    int id;
    int burst_time;
    // 必要に応じて追加
};

// @ownership: 呼び出し元がfree
ds_error_t ds_process_create(int id, int burst_time, ds_process_t **out_process) {
    if (!out_process) return DS_ERR_NULL_POINTER;
    ds_process_t *p = (ds_process_t *)malloc(sizeof(ds_process_t));
    if (!p) return DS_ERR_ALLOC;
    p->id = id;
    p->burst_time = burst_time;
    *out_process = p;
    return DS_SUCCESS;
}

ds_error_t ds_process_destroy(ds_process_t *process) {
    if (!process) return DS_ERR_NULL_POINTER;
    free(process);
    return DS_SUCCESS;
}