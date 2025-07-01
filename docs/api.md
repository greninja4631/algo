docker build -t algo-ci . && docker run --rm -it algo-ci ./run_ci.sh





CプロジェクトAPI設計・実装の統一基準
1. 型・構造体
すべての構造体型名は ds_xxx_t で統一
例：typedef struct ds_stack ds_stack_t;

関数ポインタ型は ds_xxx_func_t で統一
例：typedef void (*ds_hashmap_free_func_t)(void*);

API引数としては_tを絶対に使わないこと（例外なし）
例：ds_hashmap_create(size_t cap, ds_hashmap_free_func_t key_free, ...) のように、関数ポインタ型はそのまま使う。

2. 関数名
API関数名は ds_xxx_yyy() で統一（xxx=モジュール名, yyy=機能名）

3. エラー設計
すべてのAPIは ds_error_t 型を戻り値として返す

エラー値は DS_SUCCESS / DS_ERR_XXX で統一

モジュールごとに固有エラーは DS_ERR_XXX_YYY で拡張OK

4. 出力値・インスタンス取得の設計
関数の出力値は outポインタ経由で返す
例：ds_error_t ds_stack_pop(ds_stack_t* stack, void** out_value);

5. サイズ・インデックス
サイズ・個数・インデックス等はすべて size_t で統一

6. .hファイルとinclude設計
全API用 .hファイルは include/ds/ 配下に置く

#include "ds/xxx.h" で統一

ビルドフラグには -Iinclude -Iinclude/ds を追加

7. 命名規則の徹底
エラーenumやコールバック関数型も一貫してモジュールprefix付きで命名
例：ds_hashmap_free_func_t

8. ロガー・アロケータAPI
共通のロガー、アロケータも外部注入APIとして提供
例：void ds_set_log_function(ds_log_func_t func);

9. .h/.c設計原則
構造体本体は.cで定義し、.hはforward宣言のみ

API設計は必ずヘッダー主導（先に.hを書く）

10. Makefile/CMake
ビルドコマンドは -Iinclude -Iinclude/ds を必ず追加

全てのソースで #include "ds/xxx.h" でOKになるよう設計

11. テストディレクトリ
テストは tests/ 配下で管理、testファイルも #include "ds/xxx.h" で統一

12. その他
型名・関数名・enum名は全て「ds_」+モジュール名で衝突防止

拡張性・外部利用しやすさ・CI/CD・大規模開発・リファクタ容易性を最優先


13 共通ユーティリティ・型の標準化
複数モジュールで使う構造体やエラー/ログ型（例: ds_stats_t）は data_structures.h 等の基幹ヘッダに明示的に定義・管理。

14 標準Cヘッダのインクルード徹底
全ての .h/.c ファイルで #include <stddef.h> #include <stdbool.h> など標準型を直接 include すること。

15. API宣言・実装の型一致
.hと.cのAPIシグネチャは100%一致させる。食い違いがあればコンパイルエラーで検出される設計にする。

16. デフォルト実装・外部差し替え
外部注入API（例：ds_log, ds_malloc）はstatic関数ポインタを用意し、常にデフォルト実装を明示。

17. 実装・運用時の注意点（頻発エラーの防止ガイド）
- APIのシグネチャ（定義/宣言）は100%一致させる
- bool型利用時は #include <stdbool.h> を各ファイルで
- グローバル関数ポインタ（ds_log等）はstatic/externの管理に注意
- typedef, 構造体定義の重複禁止（基幹ヘッダで1回のみ）
- 外部注入APIはstatic関数ポインタ＋デフォルト実装で
- Makefile/CMakeで -Iinclude -Iinclude/ds を必ず追加

18. ロガー・グローバル関数ポインタの明示的定義
グローバル関数ポインタ（例：ds_log）は、必ず static な関数ポインタ変数として各.cファイルに定義すること！

例:

c
Copy
Edit
// queue.c の先頭付近
static ds_log_func_t ds_log = NULL;
static void default_log(ds_log_level_t level, const char* fmt, ...);
デフォルト実装（例：default_log）は必ずstaticで定義し、切替APIで上書きすること

19. APIシグネチャの徹底統一
.hファイルのAPI宣言と.cファイルのAPI実装は、戻り値・引数を1バイトも違えず完全一致させること

例（NG）：

.h: ds_error_t ds_stack_create(ds_stack_t** out_stack);

.c: ds_stack_t* ds_stack_create(void) { ... }
→ これは絶対NG。食い違いは即コンパイルエラー！

例（OK）：

.h: ds_error_t ds_stack_create(ds_stack_t** out_stack);

.c: ds_error_t ds_stack_create(ds_stack_t** out_stack) { ... }






## 目次

1. [エラーコード / Error Codes](#error-codes)
2. [ロギング / Logging](#logging)
3. [メモリ管理 / Memory Management](#memory-management)
4. [スタック / Stack](#stack)
5. [キュー / Queue](#queue)
6. [双方向リスト / Doubly Linked List](#doubly-linked-list)
7. [逆ポーランド記法電卓 / RPN Calculator](#rpn-calculator)
8. [ラウンドロビンスケジューラ / Round Robin Scheduler](#round-robin-scheduler)
9. [Undo/Redo履歴システム / Undo/Redo History System]

---

## 1. エラーコード / Error Codes

```c
typedef enum {
    DS_SUCCESS = 0,
    DS_ERROR_NULL_POINTER,
    DS_ERROR_OUT_OF_MEMORY,
    DS_ERROR_EMPTY_CONTAINER,
    DS_ERROR_INVALID_ARGUMENT,
    DS_ERROR_OVERFLOW,
    DS_ERROR_UNDERFLOW,
    DS_ERROR_NOT_FOUND,
    DS_ERROR_SYSTEM_FAILURE
} ds_error_t;