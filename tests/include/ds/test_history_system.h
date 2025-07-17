/* tests/include/ds/test_history_system.h
 * ------------------------------------------------------------
 * History-System モジュール専用 ― テスト用ヘルパ＆宣言
 * 2025-07 ガイドライン（Opaque 型／マイクロアーキテクチャ）準拠
 * ------------------------------------------------------------ */
#ifndef DS_TEST_HISTORY_SYSTEM_H
#define DS_TEST_HISTORY_SYSTEM_H

#include "data_structures.h"   /* ds_command_t, ds_error_t など */

/* C++ 互換 */
#ifdef __cplusplus
extern "C" {
#endif

/* ─────────────────────────────────────────────
 * テスト用コマンド初期化ヘルパ
 * 本番構造体 ds_command_t に直接値を詰めるだけ。
 * 新しいフィールドが増えてもここを直すだけで済む。
 * @ownership caller writes / library reads
 * ──────────────────────────────────────────── */
static inline void
test_command_init(ds_command_t *cmd, int id, int val)
{
    if (!cmd) return;

    /* 本番が関数ポインタ＆コンテキストのみであれば no-op */
    cmd->execute = NULL;
    cmd->undo    = NULL;
    cmd->ctx     = NULL;

    /* 将来 ds_command_t に id / val フィールドが追加されたら
       ここで代入する形にする */
    (void)id;
    (void)val;
}

/* 本番 API へ渡すときのアダプタ (キャストで警告抑止) */
#define TEST_CMD_PTR(ptr)  ((ds_command_t *)(ptr))

/* ─────────── テストスイート宣言 ─────────── */
/* 実体は tests/ds/test_history_system.c にある */
void test__history_system_basic(void);
void test__history_system_edge_cases(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DS_TEST_HISTORY_SYSTEM_H */
