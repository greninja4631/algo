#ifndef DS_PROCESS_H
#define DS_PROCESS_H

#include "data_structures.h"      /* ds_error_t など共通型 */



/* ── API ── */
/**
 * @brief プロセス生成
 * @ownership 呼び出し側が ds_process_destroy() で解放
 */
ds_error_t ds_process_create(int id, int burst_time,
                             ds_process_t **out_process);

/** @brief プロセス複製（ディープコピー） */
ds_process_t *ds_process_clone(const ds_process_t *src);

/** @brief 破棄 */
void ds_process_destroy(ds_process_t *process);

/** @brief ゲッター */
int ds_process_get_id(const ds_process_t *p);
int ds_process_get_burst_time(const ds_process_t *p);

#endif /* DS_PROCESS_H */