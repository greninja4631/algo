#ifndef DS_NEXT_NEXT_PRACTICE_H              /* ── include guard 統一 ── */
#define DS_NEXT_NEXT_PRACTICE_H

/**
 * @file   next_next_practice.h
 * @brief  雑多デモ DS/Algo 集 ― 公開 API
 *
 * 2025-07 マイクロアーキテクチャ／Opaque 型ガイドライン準拠
 */

#include "data_structures.h"   /* 前方宣言 ds_next_next_practice_t 取得 */

#ifdef __cplusplus
extern "C" {
#endif

/*───────────────────────────────────────────────────────────────
 * ファクトリ & 破棄
 *───────────────────────────────────────────────────────────────*/
/**
 * @brief  インスタンス生成
 * @param[out] out_p  生成されたハンドルを返すポインタ
 * @retval DS_SUCCESS            成功
 * @retval DS_ERR_NULL_POINTER   out_p == NULL
 * @retval DS_ERR_ALLOC          メモリ確保失敗
 * @ownership caller frees (destroy)
 */
ds_error_t ds_next_next_practice_create(ds_next_next_practice_t **out_p);

/**
 * @brief  破棄（NULL 安全）
 * @ownership library frees
 */
void       ds_next_next_practice_destroy(ds_next_next_practice_t *p);

/*───────────────────────────────────────────────────────────────
 * デモ一括実行（テスト用エントリポイントはこれ 1 つに集約）
 *───────────────────────────────────────────────────────────────*/
/**
 * @brief  内部で各種デモ／自己テストを走らせるユーティリティ
 * @note   非スレッドセーフ。戻り値で成功／失敗のみ通知。
 */
ds_error_t ds_next_next_practice_run_all_demos(ds_next_next_practice_t *p);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DS_NEXT_NEXT_PRACTICE_H */