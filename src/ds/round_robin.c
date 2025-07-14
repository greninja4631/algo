#include <stdio.h>
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "ds/round_robin.h"
#include "ds/process.h"
#include "ds/queue.h"
#include <stdlib.h>

/* --- 本体構造体（不透明型のまま） --- */
struct ds_round_robin_scheduler {
    int          time_quantum;
    ds_queue_t  *process_queue;   /* FIFO キュー */
};

/* --- スケジューラ生成 --- */
ds_error_t ds_round_robin_scheduler_create(int time_quantum,
                                           ds_round_robin_scheduler_t **out_sched)
{
    if (!out_sched)      return DS_ERR_NULL_POINTER;
    if (time_quantum<=0) return DS_ERR_INVALID_ARG;

    ds_round_robin_scheduler_t *sched = calloc(1, sizeof *sched);
    if (!sched) return DS_ERR_ALLOC;

    sched->time_quantum = time_quantum;
    ds_error_t err = ds_queue_create(&sched->process_queue);
    if (err != DS_SUCCESS) { free(sched); return err; }

    *out_sched = sched;
    return DS_SUCCESS;
}

/* --- スケジューラ破棄 --- */
ds_error_t ds_round_robin_scheduler_destroy(ds_round_robin_scheduler_t *sched)
{
    if (!sched) return DS_ERR_NULL_POINTER;

    void *proc = NULL;
    while (ds_queue_dequeue(sched->process_queue, &proc) == DS_SUCCESS)
        ds_process_destroy((ds_process_t *)proc);

    ds_queue_destroy(sched->process_queue);
    free(sched);
    return DS_SUCCESS;
}

/* --- プロセス追加 --- */
ds_error_t ds_round_robin_scheduler_add_process(ds_round_robin_scheduler_t *sched,
                                                const ds_process_t *process)
{
    if (!sched || !process) return DS_ERR_NULL_POINTER;

    ds_process_t *copy = ds_process_clone(process);
    if (!copy) return DS_ERR_ALLOC;

    ds_error_t err = ds_queue_enqueue(sched->process_queue, copy);
    if (err != DS_SUCCESS) ds_process_destroy(copy);
    return err;
}

/* --- 次のプロセス取得 --- */
ds_error_t ds_round_robin_scheduler_get_next_process(ds_round_robin_scheduler_t *sched,
                                                     ds_process_t **out_proc)
{
    if (!sched || !out_proc) return DS_ERR_NULL_POINTER;
    if (ds_queue_size(sched->process_queue) == 0) return DS_ERR_EMPTY;

    void *proc = NULL;
    ds_error_t err = ds_queue_dequeue(sched->process_queue, &proc);
    if (err != DS_SUCCESS) return err;

    *out_proc = (ds_process_t *)proc;
    /* 同じ内容をコピーして末尾へ戻す */
    ds_queue_enqueue(sched->process_queue, ds_process_clone(*out_proc));
    return DS_SUCCESS;
}

/* --- 完了プロセス削除（ID 指定） --- */
ds_error_t ds_round_robin_scheduler_complete_process(ds_round_robin_scheduler_t *sched,
                                                     int process_id)
{
    if (!sched) return DS_ERR_NULL_POINTER;

    ds_queue_t *tmp;
    ds_queue_create(&tmp);
    int found = 0;

    void *proc = NULL;
    while (ds_queue_dequeue(sched->process_queue, &proc) == DS_SUCCESS) {
        if (ds_process_get_id((ds_process_t *)proc) == process_id) {
            ds_process_destroy((ds_process_t *)proc);
            found = 1;
        } else {
            ds_queue_enqueue(tmp, proc);
        }
    }
    /* 戻し */
    while (ds_queue_dequeue(tmp, &proc) == DS_SUCCESS)
        ds_queue_enqueue(sched->process_queue, proc);

<<<<<<< HEAD
<<<<<<< HEAD
// ---------- その他 ----------
#define DS_UNUSED(x) ((void)(x))

#endif // DATA_STRUCTURES_H
>>>>>>> feature
=======
=======
    ds_queue_destroy(tmp);
>>>>>>> feature
    return found ? DS_SUCCESS : DS_ERR_NOT_FOUND;
}
>>>>>>> feature
