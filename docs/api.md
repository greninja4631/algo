docker build -t algo-ci . && docker run --rm -it algo-ci ./run_ci.sh





上記と下記のコードを既存のコードを含めて、良い部分を残し、悪いい部分を削除して省略せずに再生成して下さい。　　　

Cプロジェクト API設計・実装 ─ 完全統一ガイドライン
（2025-07 / CI&CD最適化・テスト基準対応版）

⸻

■ 1. 型・構造体・関数・命名規約
	•	すべての実体構造体は ds_<module> で宣言し、公開typedefは ds_<module>_t とする。
	•	forward用typedefだけをヘッダに置き、構造体本体は .c に隠蔽（O-DRY原則）。
	•	関数ポインタ型は ds_<module>_func_t。
	•	API名は ds_<module>_<verb>() で統一。
	•	エラーenum/コールバック型も ds_<module>_ERR_XXX / ds_<module>_callback_t に統一。
	•	グローバル変数は禁止。必要な場合は ds_<module>_g_<name> で、thread-local/mutex管理を明示。

⸻

■ 2. API設計・エラー／戻り値
	•	すべて ds_error_t 戻り。成功は DS_SUCCESS のみ。
	•	out引数は 必ず out_<name> プレフィクス。
	•	返却サイズ・個数は size_t。符号付きは int64_t。
	•	モジュール固有エラーは DS_ERR_<MODULE>_<CAUSE>。

⸻

■ 3. ヘッダ/実装/include運用
	•	include/ds/<module>.h … API宣言 & forward typedefのみ（本体定義は絶対書かない！）
	•	共通型は include/data_structures.h に一元化。
	•	外部ライブラリ（<stdio.h>等）は .c のみでinclude。
	•	#pragma once 禁止。インクルードガード必須。

⸻

■ 4. ロガー統一ルール（util/logger.h）
	•	ログ関数は ds_log() ひとつだけ。
	•	ログレベルenumは DS_LOG_LEVEL_DEBUG / INFO / WARNING / ERROR / FATAL のみ。
	•	差し替えは ds_set_log_function() だけで行う。
	•	printf族の直接呼び出しは禁止（テスト/CI専用コードを除く）。

⸻

■ 5. テスト / CI・CD / 可搬性

🟦 テスト設計・構成ルール
	•	各テスト関数は void test__<module>_<case>(void); 形式で統一
	•	例：test__stack_basic(void), test__queue_edge_cases(void)
	•	“test” のあとダブルアンダースコア __ で区切る
	•	宣言は tests/include/test_<module>.h に一元化
	•	例：void test__stack_basic(void);
	•	実装は tests/<module>/test_<module>.c のみ（実体重複厳禁）
	•	main()は tests/test_main.c のみが持つ。
	•	テストスイート呼び出しも必ず test__ 統一
	•	例：void (*ds_tests[])(void) = { test__stack_basic, test__queue_edge_cases, ... };
	•	Mock/ダミー実装は tests/mock/ に限定し、本番ビルドから除外
	•	CIで -Wall -Wextra -Werror -pedantic -fanalyzer を必ず適用
	•	Valgrind / ASan / UBSan をCIで全テストに毎push自動実行

⸻

🟦 [追加] テスト用ヘッダの厳格なルール
	•	テスト用関数宣言は必ず tests/include/test_<module>.h に集約すること
	•	→ 各 test_<module>.h は、そのモジュールのテスト関数群の唯一の宣言場所になる
	•	例：tests/include/test_stack.h, tests/include/test_queue.h など
	•	必要に応じて、tests/include/ds/ などサブディレクトリも利用可
	•	本体（例：struct ds_process { ... };）は、共通ヘッダやmain.cに“絶対に公開しない”**
	•	抽象化・隠蔽による安全性・互換性維持のため
	•	本体がどうしても必要な場合のみ、include/ds/<module>_test.h という「テスト専用ヘッダ」を作成
	•	テストファイルだけで #include する
	•	本体内容はここだけで公開。本番環境には絶対に公開しない
	•	例：#include "ds/process_test.h"
	•	または「すべてポインタ型で扱う設計」に統一し、ds_<module>_t* しか使わない運用でもOK

⸻

✅ 理由
	•	テスト設計の標準化＆メンテの一元化（どのテストが存在するかすぐにわかる）
	•	“隠蔽”＝使う側に「APIだけ」を見せ、中身（実装詳細）は隠す
	•	テストで本体アクセスが必要な場合だけ専用ヘッダでOK。本番には絶対公開しない
	•	こうすることで、中身の変更・最適化・バージョンアップでも他への影響ゼロ

⸻

■ 6. セキュリティ & パフォーマンス
	•	動的メモリ管理の“所有権”は doxygenタグ @ownership で明示
	•	memcpy/memmove にはfortifyチェック有効化（clang 17以降）
	•	32bit環境でもsize_tが32bitに収まるかCIでビルド
	•	未使用static関数は __attribute__((unused)) を付与（削除推奨）

⸻

■ 7. README/CONTRIBUTING必須事項
	•	コード規約全文・PRテンプレート・branch戦略の記載必須
	•	logger運用例・スレッド安全性・バージョン付与規則を記載
	•	CIパイプライン図（テスト→静的解析→ドキュメント→パッケージ生成）はMermaid記法で添付

⸻

💡 備考・既存コードへの反映について
	•	既存テスト関数・宣言・呼び出しは「test___」形式へ必ず統一
	•	重複実装・グローバル変数の乱立は禁止し、全ファイルでルールに準拠すること
	•	新規作成/リファクタ時は、このガイドライン最新版を最上位基準とする