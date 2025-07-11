#ifndef DS_ROUND_ROBIN_H
#define DS_ROUND_ROBIN_H

#include <stdint.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DS_ROUND_ROBIN_API_VERSION "1.0"


/**
 * @file
 * @brief ラウンドロビンスケジューラAPI
 */

// スケジューラ生成
ds_error_t ds_round_robin_scheduler_create(int time_quantum, ds_round_robin_scheduler_t** out_sched);

// スケジューラ破棄
ds_error_t ds_round_robin_scheduler_destroy(ds_round_robin_scheduler_t* scheduler);

// プロセス追加
ds_error_t ds_round_robin_scheduler_add_process(ds_round_robin_scheduler_t* scheduler, const ds_process_t* process);

// 次のプロセス取得
ds_error_t ds_round_robin_scheduler_get_next_process(ds_round_robin_scheduler_t* scheduler, ds_process_t** out_process);

// プロセス完了処理
ds_error_t ds_round_robin_scheduler_complete_process(ds_round_robin_scheduler_t* scheduler, int process_id);

#ifdef __cplusplus
}
#endif

#endif // DS_ROUND_ROBIN_H