/* tests/include/ds/test_history_system.h
 * ------------------------------------------------------------
 * History-System モジュール専用 ― テスト用ヘルパ＆宣言
 * 2025-07 ガイドライン（Opaque 型／マイクロアーキテクチャ）準拠
 * ------------------------------------------------------------ */
#ifndef DS_TEST_HISTORY_SYSTEM_H
#define DS_TEST_HISTORY_SYSTEM_H

#include "data_structures.h"   /* ds_command_t, ds_error_t など */

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
    cmd->execute = NULL;
    cmd->undo    = NULL;
    cmd->ctx     = NULL;
    (void)id;
    (void)val;
}

#define TEST_CMD_PTR(ptr)  ((ds_command_t *)(ptr))

/* ─────────── テストスイート宣言 ─────────── */
/* 必ず先頭引数に const ds_allocator_t *alloc を追加！ */
void test__history_system_basic(const ds_allocator_t *alloc);
void test__history_system_edge_cases(const ds_allocator_t *alloc);

#ifdef __cplusplus
}
#endif

#endif /* DS_TEST_HISTORY_SYSTEM_H */