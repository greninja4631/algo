#ifndef DS_NEXT_NEXT_PRACTICE_H
#define DS_NEXT_NEXT_PRACTICE_H

#include "data_structures.h"   /* ds_allocator_t / ds_error_t など */

#ifdef __cplusplus
extern "C" {
#endif


/*--------------------  API --------------------*/
/**
 * @brief 生成
 * @ownership caller must call destroy()
 */
ds_error_t ds_next_next_practice_create(
    const ds_allocator_t       *alloc,     /* 追加 */
    ds_next_next_practice_t   **out_obj);

/**
 * @brief 破棄（NULL セーフ）
 */
ds_error_t ds_next_next_practice_destroy(
    const ds_allocator_t       *alloc,     /* 追加 */
    ds_next_next_practice_t    *obj);

/**
 * @brief デモ関数群を一括実行
 */
ds_error_t ds_next_next_practice_run_all_demos(
    ds_next_next_practice_t    *obj);

#ifdef __cplusplus
}
#endif
#endif /* DS_NEXT_NEXT_PRACTICE_H */
