docker build -t algo-ci . && docker run --rm -it algo-ci ./run_ci.sh




# ✅ Cプロジェクト API設計・実装 ─ 完全統一ガイドライン
（2025年7月最新版 / CI&CD最適化・テスト基準・セキュリティ・可搬性 対応）

---

## 📘 1. 命名規約 & 型設計ルール

- 構造体（実体）：`struct ds_<module>`
- typedef名：`ds_<module>_t`
- forward typedef のみヘッダに書き、構造体本体は `.c` に隠蔽
- 関数ポインタ型：`ds_<module>_func_t`
- API名：`ds_<module>_<verb>()`
- エラー：`DS_ERR_<MODULE>_<CAUSE>`
- グローバル変数は禁止。ただし使う場合は `ds_<module>_g_<name>` にし、thread-local または mutex 管理が必要

---

## 🔁 2. API設計ポリシー

- すべての関数は `ds_error_t` を戻り値とし、成功は `DS_SUCCESS`
- 出力引数は `out_` プレフィクス付きで明示
- 個数やサイズは `size_t`、負値が必要なときは `int64_t`
- モジュールごとのエラーコードは `data_structures.h` または `<module>.h` に集約
- malloc/free の所有権は `@ownership` コメントで明示

---

## 📁 3. ヘッダ・ファイル構成ポリシー

- `include/ds/<module>.h`：API宣言、typedef（forward）のみ（構造体本体は禁止）
- `src/<module>.c`：構造体定義とAPI実装、外部ライブラリincludeもここに限定
- `include/data_structures.h`：共通型・enum定義・エラー定義の集約場所
- `tests/include/test_<module>.h`：テスト用API宣言の唯一の場所
- `include/ds/<module>_test.h`：テスト用に構造体本体が必要な場合のみ使用

> `#pragma once`は禁止 → `#ifndef` でインクルードガードを使用

---

## 📢 4. ロガー設計ルール（util/logger.h）

- 使用する関数は `ds_log()` のみ
- ログレベル：`DS_LOG_LEVEL_DEBUG`, `INFO`, `WARNING`, `ERROR`, `FATAL`
- 差し替え：`ds_set_log_function()` のみ許可
- `printf`, `fprintf`, `puts`, `perror` などの使用は禁止（テストを除く）

例：
```c
static void default_log(ds_log_level_t level, const char* fmt, ...) {
    (void)level;
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}


⸻

🧪 5. テスト設計・CI/CDポリシー

✅ テスト関数ルール
	•	形式：void test__<module>_<case>(void);
	•	ヘッダ：tests/include/test_<module>.h
	•	実装：tests/<module>/test_<module>.c
	•	test_main.c だけが main() を持つ
	•	登録形式：void (*ds_tests[])(void) = { test__stack_basic, ... };
	•	構造体本体が必要なら include/ds/<module>_test.h にのみ定義する（本番から除外）

⸻

⚙ CI/CDチェック項目
	•	コンパイル：-Wall -Wextra -Werror -pedantic -fanalyzer
	•	静的解析：clang-analyzer, cppcheck など
	•	メモリチェック：Valgrind, ASan, UBSan
	•	所有権管理：Doxygenコメント @ownership
	•	32bit対応：-m32 ビルドで size_t の安全性チェック

⸻

📂 6. CI用 Makefile / Docker（例）

Makefile 抜粋：

CFLAGS  = -Wall -Wextra -Werror -pedantic -std=c11
INCLUDES = -Iinclude -Itests/include

Docker用：

docker build -t algo-ci .
docker run --rm -it algo-ci ./run_ci.sh


⸻

🔒 7. セキュリティ & パフォーマンス強化
	•	malloc/freeの所有権は関数コメントに @ownership
	•	memcpy/memmoveはfortify付き（clang 17+）
	•	未使用 static 関数には __attribute__((unused))（または削除）
	•	クリティカル関数には __attribute__((hot)) を検討

⸻

📖 8. README / CONTRIBUTING.md の必須項目
	•	本ガイド全文 or リンクを掲載
	•	PRテンプレート / ブランチ戦略
	•	logger使用例・スレッド安全性・バージョン規約
	•	CI/CDパイプライン図（Mermaidで記述）

Mermaid例：

graph LR
    A[Push to GitHub] --> B[Build]
    B --> C[Lint & Format]
    C --> D[Test (Valgrind + ASan)]
    D --> E[Docs Generation]
    E --> F[Package & Deploy]


⸻

🔚 9. 既存コードの移行ガイド
	•	struct本体は .c に移す。ヘッダには typedef のみ
	•	グローバル変数は禁止（mutexまたはthread_localに変換）
	•	テストで構造体が必要なら、<module>_test.h を使って明示的に公開
	•	テスト関数・スイート名を test__<module>_<case> に統一
	•	再利用性のない冗長コード・重複定義は削除する

⸻

