docker build -t algo-ci . && docker run --rm -it algo-ci ./run_ci.sh



✅ エラーを解消するための「修正すべき項目リスト」

1. 全ヘッダの #include パスを正しいものに修正  
   - 例:  
     - `#include "../data_structures.h"` → `#include "data_structures.h"`
     - `#include "lru_cache.h"` → `#include "ds/lru_cache.h"` など  
     （Makefileの `-I` で `include` ディレクトリが通っている前提）

2. 必要なヘッダファイルが本当に存在しているか確認  
   - 例: `include/ds/lru_cache.h`, `include/ds/circular_list.h` など

3. すべての「不透明型」宣言をヘッダで行う  
   - 例:  
     ```c
     typedef struct ds_lru_cache ds_lru_cache_t;
     ```
     - 使用する型・関数は必ず正しいヘッダで宣言/include

4. 型や関数プロトタイプ宣言の漏れをなくす  
   - `Statistics` 型、`calculate_statistics()` などは使う前に必ず宣言/include

5. C標準型（bool, size_tなど）を使うファイルでヘッダを追加  
   - 例:  
     - `#include <stdbool.h>`
     - `#include <stddef.h>`

6. 同名関数・変数の二重定義を避ける  
   - 例:  
     - `ds_log` のグローバル関数とstatic変数名の衝突を解消  
     - 名前を変える、もしくはstatic変数を削除

7. `LOG_ERROR` などの独自マクロやロギングを `ds_log` に統一する

8. `ds_stats_t` など不明な型を使う場合は型定義する  
   - もしくは、使わない実装に変更する

9. `printf` 未定義エラーの場合は `<stdio.h>` を必ず include

10. MakefileやDockerfileの `-I` で `include` フォルダが必ず通っているか確認


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