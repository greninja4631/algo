#include "ds/process.h"
#include "util/memory.h"   /* ds_malloc / ds_free */

/**
 * @file    process.c
 * @brief   プロセス構造体のOpaque実装（2025ガイドライン準拠）
 * @note    オーナーシップ・解放責任の明示／アロケータDI対応
 */

/* Opaque本体はsrc限定 */
struct ds_process {
    int32_t pid;
    int32_t burst;
};

/**
 * @brief プロセス生成
 * @param[in]  alloc   アロケータ
 * @param[in]  pid     プロセスID
 * @param[in]  burst   バーストタイム
 * @param[out] out_proc 新規プロセスハンドル
 * @return DS_SUCCESS等
 * @ownership caller frees via ds_process_destroy
 */
ds_error_t ds_process_create(const ds_allocator_t *alloc,
                             int32_t               pid,
                             int32_t               burst,
                             ds_process_t        **out_proc)
{
    if (!alloc || !out_proc) return DS_ERR_NULL_POINTER;

    ds_process_t *p = ds_malloc(alloc, 1, sizeof *p);
    if (!p) return DS_ERR_ALLOC;

    p->pid   = pid;
    p->burst = burst;
    *out_proc = p;
    return DS_SUCCESS;
}

/**
 * @brief プロセス破棄
 * @param[in] alloc アロケータ
 * @param[in] p     プロセス
 * @return DS_SUCCESS等
 * @ownership callee frees
 */
ds_error_t ds_process_destroy(const ds_allocator_t *alloc, ds_process_t *p)
{
    if (!alloc) return DS_ERR_NULL_POINTER;
    if (!p)     return DS_SUCCESS;  /* NULLセーフ設計 */
    ds_free(alloc, p);
    return DS_SUCCESS;
}

/* --- Getter APIはカプセル化厳守 --- */
int32_t ds_process_get_id(const ds_process_t *p)         { return p ? p->pid   : -1; }
int32_t ds_process_get_burst_time(const ds_process_t *p) { return p ? p->burst : -1; }