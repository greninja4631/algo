#ifndef DS_ROUND_ROBIN_H
#define DS_ROUND_ROBIN_H

#include <stdint.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DS_ROUND_ROBIN_API_VERSION "1.0"

/**
 * @file ds/round_robin.h
 * @brief ラウンドロビンスケジューラ Opaque型＋抽象アロケータDI対応API
 */

/* Opaque型前方宣言は data_structures.h だけ */

/**
 * @brief スケジューラ生成
 * @ownership caller frees (destroy)
 */
ds_error_t ds_round_robin_scheduler_create(
    const ds_allocator_t* alloc,
    int time_quantum,
    ds_round_robin_scheduler_t** out_sched
);

/**
 * @brief スケジューラ破棄
 */
ds_error_t ds_round_robin_scheduler_destroy(
    const ds_allocator_t* alloc,
    ds_round_robin_scheduler_t* scheduler
);

/**
 * @brief プロセス追加
 */
ds_error_t ds_round_robin_scheduler_add_process(
    const ds_allocator_t* alloc,
    ds_round_robin_scheduler_t* scheduler,
    const ds_process_t* process
);

/**
 * @brief 次のプロセス取得
 */
ds_error_t ds_round_robin_scheduler_get_next_process(
    const ds_allocator_t* alloc,
    ds_round_robin_scheduler_t* scheduler,
    ds_process_t** out_process
);

/**
 * @brief プロセス完了処理
 */
ds_error_t ds_round_robin_scheduler_complete_process(
    const ds_allocator_t* alloc,
    ds_round_robin_scheduler_t* scheduler,
    int process_id
);

#ifdef __cplusplus
}
#endif

#endif // DS_ROUND_ROBIN_H
