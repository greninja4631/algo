#ifndef DS_ROUND_ROBIN_H
#define DS_ROUND_ROBIN_H
#include <stdint.h>
#include <stddef.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DS_ROUND_ROBIN_API_VERSION  "1.1"   /* ↑バージョンだけ更新 */
#define DS_RR_HAS_SIZE_API          1       /* テスト側フラグ      */

/*―― Opaque 型は data_structures.h に前方宣言 ――*/

/* create/destroy --------------------------------------------------------*/
/** @ownership caller frees (→ destroy) */
ds_error_t ds_round_robin_scheduler_create(
        const ds_allocator_t        *alloc,
        int32_t                      time_quantum,
        ds_round_robin_scheduler_t **out_sched);

ds_error_t ds_round_robin_scheduler_destroy(
        const ds_allocator_t        *alloc,
        ds_round_robin_scheduler_t  *sched);

/* enqueue / dequeue -----------------------------------------------------*/
/** @ownership transfer → scheduler */
ds_error_t ds_round_robin_scheduler_add_process(
        const ds_allocator_t        *alloc,
        ds_round_robin_scheduler_t  *sched,
        const ds_process_t          *proc);

/** @ownership transfer ← caller (pop したら caller が destroy) */
ds_error_t ds_round_robin_scheduler_get_next_process(
        const ds_allocator_t        *alloc,
        ds_round_robin_scheduler_t  *sched,
        ds_process_t               **out_proc);

/* optional helpers ------------------------------------------------------*/
size_t ds_round_robin_scheduler_size(
        const ds_round_robin_scheduler_t *sched);

/* ――旧 API 名は当面マクロで残す（ビルド切れ防止）―― */
#define ds_round_robin_scheduler_enqueue  ds_round_robin_scheduler_add_process
#define ds_round_robin_scheduler_dequeue  ds_round_robin_scheduler_get_next_process

#ifdef __cplusplus
}
#endif
#endif /* DS_ROUND_ROBIN_H */