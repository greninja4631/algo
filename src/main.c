/**
 * @file    src/main.c
 * @brief   ライブラリ実行バイナリのエントリーポイント
 *
 * ──────────────────────────────────────────────
 *  ビルド時フラグ
 *  --------------------------------------------
 *  • -DTESTING  を付けると  main() を除外（単体テスト用）
 *
 *  ガイドライン適合ポイント
 *  --------------------------------------------
 *  ✔ 標準 C シグネチャ      : int main(int argc, char *argv[])
 *  ✔ 直接 printf 禁止       : ds_log() でログ出力
 *  ✔ 依存性逆転 (DI)        : アロケータは ds_get_default_allocator() で取得想定
 *  ✔ グローバル状態なし     : 必要なら静的ローカルで管理
 *  ✔ 戻り値 0 = 正常終了
 * ──────────────────────────────────────────────
 */

#include "data_structures.h"   /* DS_API_VERSION ほか共通型 */
#include "util/logger.h"       /* ds_log()                 */
#include "util/memory.h"       /* ds_get_default_allocator (実装側で用意されている前提) */

#include <stdio.h>   /* (void)argc 等の警告抑制にだけ使用。入出力はロガーへ */

#ifdef __cplusplus
extern "C" {
#endif

/*======================================================================*/
/*  production main                                                     */
/*======================================================================*/
#ifndef TESTING        /* 単体テストビルド時は main を含めない  */
int main(int argc, char *argv[])
{
    /* ---- 未使用警告抑制（将来 CLI 解析を追加する場合は削除） ---- */
    (void)argc;
    (void)argv;

    /* ---- 依存性逆転：デフォルトアロケータを取得 ----
       プロジェクトで ds_get_default_allocator() が未実装なら
       NULL を使い、下層で標準 malloc/free が利用される */
#ifdef DS_HAVE_DEFAULT_ALLOCATOR
    const ds_allocator_t *alloc = ds_get_default_allocator();
#else
    const ds_allocator_t *alloc = NULL;
#endif
    (void)alloc;  /* いまは未使用 */

    /* ---- 起動ログ ---- */
    ds_log(DS_LOG_LEVEL_INFO,
           "Data-Structures Library %s — application start",
           DS_API_VERSION);

    /* ==============================================================
     * TODO: ここにアプリ本体の初期化 / CLI 解析 / サブコマンド呼び出し
     *       などを追加してください。
     * ============================================================== */

    /* ---- 正常終了ログ ---- */
    ds_log(DS_LOG_LEVEL_INFO, "Shutdown complete. Goodbye!");
    return 0;
}
#endif /* TESTING */

#ifdef __cplusplus
} /* extern "C" */
#endif