
#ifndef DS_NEXT_NEXT_PRACTICE_H
#define DS_NEXT_NEXT_PRACTICE_H
/**
 * @file    include/ds/next_next_practice.h
 * @brief   “次々プラクティス”デモ・ユーティリティ  ─ Opaque 型＋抽象アロケータ DI
 * @version 2025-07
 *
 * ──────────────────────────────────────────────────────────
 *  ガイドライン要点
 *  • すべての公開 API の第 1 引数は const ds_allocator_t *alloc 固定
 *  • @ownership を必ず記述（caller frees / callee frees / transfer）
 *  • NULL セーフ：alloc==NULL でも ds_malloc/ds_free にフォールバック
 *  • 戻り値は ds_error_t（成功は DS_SUCCESS, 失敗は DS_ERR_*）
 * ──────────────────────────────────────────────────────────
 */

#include "data_structures.h" /* ds_allocator_t / ds_error_t / forward decls */

#ifdef __cplusplus
extern "C" {
#endif

/* ──────────────────────────────
 * Opaque 本体は .c 内でのみ定義
 * typedef struct ds_next_next_practice ds_next_next_practice_t;
 * （↑ は data_structures.h に集中宣言済み）
 * ────────────────────────────── */

/*--------------------------------------------------------------
 * API
 *-------------------------------------------------------------*/

/**
 * @brief インスタンス生成
 * @param[in]  alloc     抽象アロケータ（NULL → ds_malloc フォールバック）
 * @param[out] out_obj   生成されたハンドル
 * @retval DS_SUCCESS            正常終了
 * @retval DS_ERR_NULL_POINTER   引数が NULL
 * @retval DS_ERR_ALLOC          メモリ確保失敗
 * @ownership caller frees （ds_next_next_practice_destroy で解放）
 */
ds_error_t ds_next_next_practice_create(
    const ds_allocator_t        *alloc,
    ds_next_next_practice_t    **out_obj);

/**
 * @brief インスタンス破棄（NULL セーフ・冪等）
 * @param[in] alloc   抽象アロケータ
 * @param[in] obj     ハンドル（NULL の場合 DS_SUCCESS を返す）
 * @retval DS_SUCCESS            正常終了
 * @retval DS_ERR_NULL_POINTER   alloc が NULL
 * @ownership callee frees
 */
ds_error_t ds_next_next_practice_destroy(
    const ds_allocator_t        *alloc,
    ds_next_next_practice_t     *obj);

/**
 * @brief 例示デモを一括実行
 * @param[in] alloc 抽象アロケータ
 * @param[in] obj   ハンドル
 * @retval DS_SUCCESS            正常終了
 * @retval DS_ERR_NULL_POINTER   alloc または obj が NULL
 * @note   内部で Undo/Redo・Stack/Queue 等のデモを実行し、
 *         ログへ結果を出力するだけで外部状態は変更しません。
 */
ds_error_t ds_next_next_practice_run_all_demos(
    const ds_allocator_t        *alloc,
    ds_next_next_practice_t     *obj);

#ifdef __cplusplus
}
#endif
#endif /* DS_NEXT_NEXT_PRACTICE_H */