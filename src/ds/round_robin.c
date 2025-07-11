#include "ds/round_robin.h"
#include "ds/process.h"
#include "ds/queue.h"
#include <stdlib.h>
#include <string.h>


/* --- processのディープコピー --- */
static ds_process_t* _clone_process(const ds_process_t* src) {
    if (!src) return NULL;
    ds_process_t* p = malloc(sizeof(ds_process_t));
    if (!p) return NULL;
    memcpy(p, src, sizeof(ds_process_t));
    return p;
}

/* --- 本体構造体 --- */
struct ds_round_robin_scheduler {
    int time_quantum;
    ds_queue_t* process_queue; // プロセスキュー（FIFO）
};

/* --- スケジューラ生成 --- */
ds_error_t ds_round_robin_scheduler_create(int time_quantum, ds_round_robin_scheduler_t** out_sched) {
    if (!out_sched) return DS_ERR_NULL_POINTER;
    if (time_quantum <= 0) return DS_ERR_INVALID_ARG;

    ds_round_robin_scheduler_t* sched = calloc(1, sizeof(*sched));
    if (!sched) return DS_ERR_ALLOC;
    sched->time_quantum = time_quantum;

    ds_error_t err = ds_queue_create(&sched->process_queue);
    if (err != DS_SUCCESS) {
        free(sched);
        return err;
    }
    *out_sched = sched;
    return DS_SUCCESS;
}

/* --- スケジューラ破棄 --- */
ds_error_t ds_round_robin_scheduler_destroy(ds_round_robin_scheduler_t* scheduler) {
    if (!scheduler) return DS_ERR_NULL_POINTER;

    // キューに残った全プロセスをfree
    void* proc = NULL;
    while (ds_queue_dequeue(scheduler->process_queue, &proc) == DS_SUCCESS) {
        free(proc);
    }
    ds_queue_destroy(scheduler->process_queue);
    free(scheduler);
    return DS_SUCCESS;
}

/* --- プロセス追加 --- */
ds_error_t ds_round_robin_scheduler_add_process(ds_round_robin_scheduler_t* scheduler, const ds_process_t* process) {
    if (!scheduler || !process) return DS_ERR_NULL_POINTER;

    ds_process_t* p_copy = _clone_process(process);
    if (!p_copy) return DS_ERR_ALLOC;

    ds_error_t err = ds_queue_enqueue(scheduler->process_queue, p_copy);
    if (err != DS_SUCCESS) {
        free(p_copy);
        return err;
    }
    return DS_SUCCESS;
}

/* --- 次のプロセス取得（ラウンドロビン: 先頭をpopし末尾にpush） --- */
ds_error_t ds_round_robin_scheduler_get_next_process(ds_round_robin_scheduler_t* scheduler, ds_process_t** out_process) {
    if (!scheduler || !out_process) return DS_ERR_NULL_POINTER;
    if (ds_queue_size(scheduler->process_queue) == 0) return DS_ERR_EMPTY;

    void* proc = NULL;
    ds_error_t err = ds_queue_dequeue(scheduler->process_queue, &proc);
    if (err != DS_SUCCESS) return err;

    // プロセスを返し、末尾に戻す
    *out_process = proc;
    ds_queue_enqueue(scheduler->process_queue, _clone_process((ds_process_t*)proc));
    free(proc); // ユーザには「新規コピー」で返す方式

    return DS_SUCCESS;
}

/* --- プロセス完了処理（ID指定で削除） --- */
ds_error_t ds_round_robin_scheduler_complete_process(ds_round_robin_scheduler_t* scheduler, int process_id) {
    if (!scheduler) return DS_ERR_NULL_POINTER;
    if (ds_queue_size(scheduler->process_queue) == 0) return DS_ERR_EMPTY;

    // 一時バッファで再構築
    ds_queue_t* tmpq;
    ds_queue_create(&tmpq);
    void* proc = NULL;
    int found = 0;
    while (ds_queue_dequeue(scheduler->process_queue, &proc) == DS_SUCCESS) {
        ds_process_t* p = (ds_process_t*)proc;
        if (p->id == process_id) {
            free(p); // 削除
            found = 1;
        } else {
            ds_queue_enqueue(tmpq, p);
        }
    }
    // tmpq → process_queueに戻す
    while (ds_queue_dequeue(tmpq, &proc) == DS_SUCCESS)
        ds_queue_enqueue(scheduler->process_queue, proc);
    ds_queue_destroy(tmpq);

    return found ? DS_SUCCESS : DS_ERR_NOT_FOUND;
}