<<<<<<< HEAD
#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include "../data_structures.h" // ds_process_t, ds_error_t など共通型の利用
=======
#ifndef DS_ROUND_ROBIN_H
#define DS_ROUND_ROBIN_H

#include <stdint.h>
#include "data_structures.h"
>>>>>>> feature

#ifdef __cplusplus
extern "C" {
#endif

<<<<<<< HEAD
typedef struct ds_round_robin_scheduler ds_round_robin_scheduler_t;

/**
 * @brief ラウンドロビンスケジューラを生成
 * @param time_quantum タイムクォンタム（ms単位など）
 * @return スケジューラ構造体へのポインタ（失敗時NULL）
 */
ds_round_robin_scheduler_t* ds_round_robin_scheduler_create(int time_quantum);

/**
 * @brief スケジューラ破棄（全プロセスと本体メモリを解放）
 * @param scheduler
 * @return エラーコード
 */
ds_error_t ds_round_robin_scheduler_destroy(ds_round_robin_scheduler_t* scheduler);

/**
 * @brief プロセス追加
 * @param scheduler
 * @param process 追加するプロセス情報
 * @return エラーコード
 */
ds_error_t ds_round_robin_scheduler_add_process(ds_round_robin_scheduler_t* scheduler, const ds_process_t* process);

/**
 * @brief 次に実行するプロセス情報を取得（スケジューリングを進める）
 * @param scheduler
 * @param process_out 次にスケジューリングされるプロセス情報（出力用）
 * @return エラーコード
 */
ds_error_t ds_round_robin_scheduler_get_next_process(ds_round_robin_scheduler_t* scheduler, ds_process_t* process_out);

/**
 * @brief プロセス実行完了（指定IDのプロセスを終了扱いに）
 * @param scheduler
 * @param process_id 完了したプロセスID
 * @return エラーコード
 */
ds_error_t ds_round_robin_scheduler_complete_process(ds_round_robin_scheduler_t* scheduler, int process_id);

/**
 * @brief 現在のプロセス数を取得
 * @param scheduler
 * @return size_t プロセス数
 */
size_t ds_round_robin_scheduler_size(const ds_round_robin_scheduler_t* scheduler);

=======
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

>>>>>>> feature
#ifdef __cplusplus
}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#endif // ROUND_ROBIN_H
=======
#endif // DS_ROUND_ROBIN_H
>>>>>>> feature
=======
#endif // DS_ROUND_ROBIN_H
>>>>>>> feature
