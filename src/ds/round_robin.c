// src/ds/round_robin.c
#include "../../include/data_structures.h"
#include "../../include/ds/queue.h"         // ← これを必ず追加！
#include "../../include/ds/round_robin.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// ---- プロセス構造体の例 ----
// ※本来は include/data_structures.h で typedef struct ds_process_t など定義済み想定

// ---- ラウンドロビンスケジューラ本体 ----

struct ds_round_robin_scheduler {
    int time_quantum;
    ds_queue_t* queue;
};

ds_round_robin_scheduler_t* ds_round_robin_scheduler_create(int time_quantum) {
    ds_round_robin_scheduler_t* sched = (ds_round_robin_scheduler_t*)ds_malloc(sizeof(ds_round_robin_scheduler_t));
    if (!sched) {
        ds_log(DS_LOG_ERROR, "[ds_round_robin_scheduler_create] メモリ確保失敗\n");
        return NULL;
    }
    sched->queue = ds_queue_create();
    if (!sched->queue) {
        ds_log(DS_LOG_ERROR, "[ds_round_robin_scheduler_create] キュー確保失敗\n");
        ds_free(sched);
        return NULL;
    }
    sched->time_quantum = time_quantum;
    return sched;
}

ds_error_t ds_round_robin_scheduler_destroy(ds_round_robin_scheduler_t* sched) {
    if (!sched) return DS_ERR_NULL_POINTER;
    ds_queue_destroy(sched->queue);
    ds_free(sched);
    return DS_SUCCESS;
}

ds_error_t ds_round_robin_scheduler_add_process(ds_round_robin_scheduler_t* sched, const ds_process_t* process) {
    if (!sched || !process) return DS_ERR_NULL_POINTER;
    ds_process_t* proc_copy = (ds_process_t*)ds_malloc(sizeof(ds_process_t));
    if (!proc_copy) {
        ds_log(DS_LOG_ERROR, "[ds_round_robin_scheduler_add_process] メモリ確保失敗\n");
        return DS_ERR_ALLOC;
    }
    memcpy(proc_copy, process, sizeof(ds_process_t));
    return ds_queue_enqueue(sched->queue, proc_copy);
}

// 次のプロセスを取得（先頭をdequeueして、末尾に再度enqueue＝ラウンドロビン）
ds_error_t ds_round_robin_scheduler_get_next_process(ds_round_robin_scheduler_t* sched, ds_process_t* process_out) {
    if (!sched || !process_out) return DS_ERR_NULL_POINTER;
    void* proc_ptr = NULL;
    ds_error_t err = ds_queue_dequeue(sched->queue, &proc_ptr);
    if (err != DS_SUCCESS) return err;
    memcpy(process_out, proc_ptr, sizeof(ds_process_t));
    // ※終了していないなら再度末尾に入れる
    ds_queue_enqueue(sched->queue, proc_ptr);
    // ※終了プロセスの場合、freeしたいときはds_round_robin_scheduler_complete_processを呼ぶ
    return DS_SUCCESS;
}

// 指定プロセスをスケジューラから削除（終了処理などに）
ds_error_t ds_round_robin_scheduler_complete_process(ds_round_robin_scheduler_t* sched, int process_id) {
    if (!sched) return DS_ERR_NULL_POINTER;
    size_t sz = ds_queue_size(sched->queue);
    ds_error_t err = DS_ERR_NOT_FOUND;
    for (size_t i = 0; i < sz; ++i) {
        void* ptr = NULL;
        ds_queue_dequeue(sched->queue, &ptr);
        if (ptr && ((ds_process_t*)ptr)->process_id == process_id) {
            ds_free(ptr); // 見つけたプロセスはfree
            err = DS_SUCCESS;
        } else {
            ds_queue_enqueue(sched->queue, ptr); // まだ終了しないものは再度末尾へ
        }
    }
    return err;
}

// ------ 学習メモ ------
// ・メモリは「ds_malloc/ds_free」だけで統一
// ・ログは「ds_log」だけで統一
// ・malloc/free, log_func, staticポインタ等は一切使わない
// ・依存性注入により、CI/テスト/本番で柔軟に切り替え可
