#ifndef DS_PROCESS_H
#define DS_PROCESS_H

#include "data_structures.h"   /* ds_allocator_t / ds_error_t */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   プロセス生成（PID・バーストタイムを指定）
 * @details 所有権はcallerにtransfer、destroy必須
 * @ownership caller frees
 */
ds_error_t ds_process_create(
    const ds_allocator_t *alloc,
    int32_t               pid,
    int32_t               burst_time,
    ds_process_t        **out_process);

/**
 * @brief   プロセス破棄（NULL-safe/冪等）
 * @details 何度呼んでも安全。free(NULL)完全対応。
 * @ownership caller frees
 */
ds_error_t ds_process_destroy(
    const ds_allocator_t *alloc,
    ds_process_t         *proc);

/**
 * @brief   プロセスID取得
 * @note    不変条件：NULL時は0返却 or assert
 */
int32_t ds_process_get_id(const ds_process_t *proc);

/**
 * @brief   バーストタイム取得
 * @note    不変条件：NULL時は0返却 or assert
 */
int32_t ds_process_get_burst_time(const ds_process_t *proc);

#ifdef __cplusplus
}
#endif
#endif /* DS_PROCESS_H */
