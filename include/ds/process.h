#ifndef DS_PROCESS_H
#define DS_PROCESS_H

#include "data_structures.h"  // ds_process_t 取得

#ifdef __cplusplus
extern "C" {
#endif

// API関数宣言のみ
ds_error_t ds_process_create(int id, int burst_time, ds_process_t **out_process);
ds_error_t ds_process_destroy(ds_process_t *process);

#ifdef __cplusplus
}
#endif

#endif // DS_PROCESS_H