/**
 * @file    src/main.c
 * @brief   ライブラリ／アプリ本体バイナリの「唯一の」エントリポイント
 *
 * ──────────────────────────────────────────────
 *  このmain()は「実運用用」。test_main.cとは完全に分離。
 *  - ガイドライン完全順守: 再帰定義・FSM設計・DI・アロケータ・ロギング
 *  - CLI, AI, DB, VM等の拡張点をコメントで明示
 *  - CI/CDでvalgrind, 静的解析, フォーマット, 禁止API grep全部Pass前提
 *  - ロギングはds_log()/logger.c経由のみ
 * ──────────────────────────────────────────────
 */

#include "data_structures.h"      // APIバージョン, Opaque型, 共通定義
#include "util/logger.h"          // ds_log()
#include "util/memory.h"          // ds_get_default_allocator()

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TESTING  // CIテスト時は除外
int main(int argc, char *argv[])
{
    (void)argc; (void)argv;  // 未使用警告抑止

    // ---- DI: アロケータ取得 ----
    const ds_allocator_t *alloc =
#ifdef DS_HAVE_DEFAULT_ALLOCATOR
        ds_get_default_allocator();
#else
        NULL; // Fallback（下層で標準malloc/free）
#endif
    (void)alloc; // 現状未使用。全APIの第1引数がallocであること！

    // ---- 起動ログ ----
    ds_log(DS_LOG_LEVEL_INFO, "Data-Structures Library v%s — Startup", DS_API_VERSION);

    // ================= CLI/AI/VM/DB/Cloud/Undo-Redo起動点 ===================
    // 例:
    //    cli_dispatch(argc, argv, alloc);
    //    ai_model_bootstrap(alloc);
    //    db_migrate_if_needed();
    //    cloud_service_start(...);
    //    undo_redo_system_init();
    // ========================================================================

    ds_log(DS_LOG_LEVEL_INFO, "Shutdown complete. Goodbye!");
    return 0;
}
#endif // TESTING

#ifdef __cplusplus
}
#endif