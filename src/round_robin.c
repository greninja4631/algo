/**
 * @file round_robin.c
 * @brief Round Robin Scheduler ADT Implementation
 */

 #include "data_structures.h"
 #include <stdlib.h>
 #include <string.h>
 #include <stdarg.h>
 #include <stdio.h>  // stderr, vfprintf必須
 
 // .hで宣言された本体のみに限る！型重複なし
 struct ds_round_robin_scheduler {
     int time_quantum;
     ds_queue_t* queue;
 };
 
 // 依存性注入のための関数ポインタ（static変数）
 static ds_malloc_func_t ds_malloc = malloc;
 static ds_free_func_t ds_free = free;
 
 // デフォルトログ関数
 static void default_log(ds_log_level_t level, const char* fmt, ...) {
     (void)level;
     va_list args;
     va_start(args, fmt);
     vfprintf(stderr, fmt, args);
     va_end(args);
 }
 static ds_log_func_t ds_log = default_log;
 
 // プロセス構造体のディープコピー（内部のみ利用）
 static ds_process_t* clone_process(const ds_process_t* src) {
     if (!src) return NULL;
     ds_process_t* copy = (ds_process_t*)ds_malloc(sizeof(ds_process_t));
     if (!copy) return NULL;
     memcpy(copy, src, sizeof(ds_process_t));
     return copy;
 }
 
 /*----------------- 公開API実装 --------------------*/
 
 ds_round_robin_scheduler_t* ds_round_robin_scheduler_create(int time_quantum) {
     if (time_quantum <= 0) return NULL;
     ds_round_robin_scheduler_t* sched = (ds_round_robin_scheduler_t*)ds_malloc(sizeof(ds_round_robin_scheduler_t));
     if (!sched) {
         ds_log(DS_LOG_ERROR, "[ds_round_robin_scheduler_create] Memory allocation failed\n");
         return NULL;
     }
     sched->queue = ds_queue_create();
     if (!sched->queue) {
         ds_free(sched);
         return NULL;
     }
     sched->time_quantum = time_quantum;
     return sched;
 }
 
 ds_error_t ds_round_robin_scheduler_destroy(ds_round_robin_scheduler_t* scheduler) {
     if (!scheduler) return DS_ERR_NULL_POINTER;
     void* proc;
     ds_error_t ret;
     while ((ret = ds_queue_dequeue(scheduler->queue, &proc)) == DS_SUCCESS) {
         ds_free(proc);
     }
     // queueが空でなければ異常
     if (ret != DS_ERR_EMPTY) {
         ds_queue_destroy(scheduler->queue);
         ds_free(scheduler);
         return ret;
     }
     ds_queue_destroy(scheduler->queue);
     ds_free(scheduler);
     return DS_SUCCESS;
 }
 
 ds_error_t ds_round_robin_scheduler_add_process(ds_round_robin_scheduler_t* scheduler, const ds_process_t* process) {
     if (!scheduler || !process) return DS_ERR_NULL_POINTER;
     ds_process_t* proc_copy = clone_process(process);
     if (!proc_copy) return DS_ERR_ALLOC;
     return ds_queue_enqueue(scheduler->queue, proc_copy);
 }
 
 ds_error_t ds_round_robin_scheduler_get_next_process(ds_round_robin_scheduler_t* scheduler, ds_process_t* process_out) {
     if (!scheduler || !process_out) return DS_ERR_NULL_POINTER;
     void* proc_ptr;
     if (ds_queue_dequeue(scheduler->queue, &proc_ptr) != DS_SUCCESS) {
         return DS_ERR_EMPTY;
     }
 
     ds_process_t* proc = (ds_process_t*)proc_ptr;
     int exec_time = (proc->remaining_time < scheduler->time_quantum)
                         ? proc->remaining_time
                         : scheduler->time_quantum;
     proc->remaining_time -= exec_time;
     memcpy(process_out, proc, sizeof(ds_process_t));
 
     if (proc->remaining_time > 0) {
         ds_queue_enqueue(scheduler->queue, proc);
     } else {
         ds_free(proc);
     }
     return DS_SUCCESS;
 }
 
 ds_error_t ds_round_robin_scheduler_complete_process(ds_round_robin_scheduler_t* scheduler, int process_id) {
     if (!scheduler) return DS_ERR_NULL_POINTER;
     ds_queue_t* queue = scheduler->queue;
     size_t n = ds_queue_size(queue);
     bool found = false;
     for (size_t i = 0; i < n; ++i) {
         void* proc_ptr;
         if (ds_queue_dequeue(queue, &proc_ptr) != DS_SUCCESS) break;
         ds_process_t* proc = (ds_process_t*)proc_ptr;
         if (proc->process_id == process_id) {
             ds_free(proc);
             found = true;
             continue;
         }
         ds_queue_enqueue(queue, proc);
     }
     return found ? DS_SUCCESS : DS_ERR_NOT_FOUND;
 }
 
 /*----------------- 依存性注入API（テスト/差し替え用） --------------------*/
 void ds_set_log_function(ds_log_func_t func) {
     ds_log = func ? func : default_log;
 }
 void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func) {
     ds_malloc = malloc_func ? malloc_func : malloc;
     ds_free = free_func ? free_func : free;
 }