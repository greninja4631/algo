/**
 * @file config.h
 * @brief 全データ構造ライブラリ共通のグローバル設定ファイル
 * @version 1.0.0
 * @date 2025-05-28
 *
 * 【設計理念】
 * - 本番/テスト/CI/CD/クラウドなどの環境切り替え、挙動チューニングを一元管理
 * - マジックナンバー・環境依存値・ON/OFFスイッチを集約し、運用/保守/変更を最小コスト化
 * - 全ソースで #include することで、将来の運用/チューニング/バージョン管理にも強くなる
 * - **ヘッダガード/ifdef化で多重インクルード・衝突を防止**
 */

 #ifndef CONFIG_H
 #define CONFIG_H

 /* -----------------------
  * バージョン・著作権情報
  * -----------------------
  */
 #define DS_LIBRARY_VERSION "1.0.0"
 #define DS_LIBRARY_AUTHOR  "OpenAI Silicon Valley Template"
 #define DS_LIBRARY_COPYRIGHT "(c) 2025 Takumi Matsuda, OpenAI All Rights Reserved."
 
 /* -----------------------
  * 環境切り替えフラグ
  * - 本番運用・テスト・CI/CD等で挙動変更
  * -----------------------
  */
 
 // デバッグビルドか（開発時：1／本番：0）
 #ifndef DS_DEBUG
 #define DS_DEBUG 1
 #endif


 typedef struct ds_config ds_config_t;

ds_config_t* ds_config_create(void);
void ds_config_destroy(ds_config_t* config);
 
 
 // クラウド監視用メトリクス出力（ON:1/OFF:0）
 #ifndef DS_ENABLE_METRICS
 #define DS_ENABLE_METRICS 1
 #endif
 
 // 標準ログ出力（ファイル/クラウド/本番切替用）
 #ifndef DS_LOG_TO_STDERR
 #define DS_LOG_TO_STDERR 1
 #endif
 
 // メモリ安全チェック強化（Valgrind/ASAN用）
 #ifndef DS_MEMORY_SAFETY
 #define DS_MEMORY_SAFETY 1
 #endif
 
 /* -----------------------
  * 重要マジックナンバー・パラメータ
  * -----------------------
  */
 
 // スタック・キュー・リストなどのデフォルト初期サイズ
 #define DS_DEFAULT_INITIAL_CAPACITY 16
 
 // Undo/Redo履歴のデフォルト最大数（0=無制限）
 #define DS_DEFAULT_MAX_HISTORY 100
 
 // RPN電卓用数値バッファ最大長
 #define DS_RPN_BUFFER_SIZE 128
 
 // ラウンドロビン・タイムクォンタム（ms, デフォルト値）
 #define DS_DEFAULT_TIME_QUANTUM 50
 
 /* -----------------------
  * 安全マクロ（NULL/エラー/不要警告の防止）
  * -----------------------
  */
 #ifndef UNUSED
 #define UNUSED(x) (void)(x)
 #endif
 
 /* -----------------------
  * プラットフォーム依存コード対策
  * -----------------------
  */
 #if defined(_WIN32) || defined(_WIN64)
   #define DS_PATH_SEPARATOR '\\'
 #else
   #define DS_PATH_SEPARATOR '/'
 #endif

 /* -----------------------
  * その他将来拡張フック・外部サービス連携
  * -----------------------
  * 例: #define DS_USE_PROMETHEUS 1
  *     #define DS_USE_GCP_MONITORING 0
  */
 
 /* -----------------------
  * 初心者向けメモ
  * -----------------------
  * - 「config.h」を修正→再ビルドすれば全体に即時反映
  * - 機能追加・挙動調整も1ファイル編集でOK（分散管理よりミスが減る）
  */
 
 #endif // CONFIG_H