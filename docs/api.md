# ① 変更を反映したDockerイメージを必ず再ビルド

docker build -t algo-ci . && docker run --rm -it algo-ci ./run_ci.sh

上記と同じ変更必要要件を満たすようなコードを既存のコードを含めて100%完全版を作成してください。

ヘッダ宣言と.cファイルの型・関数名・引数を完全一致させる

やるべきこと:

ヘッダと.cで「戻り値／引数／関数名／型名」を完全一致させる

例（どちらも↓の形に統一）
ds_hashmap_t* ds_hashmap_create(size_t capacity, ds_hashmap_free_func key_free, ds_hashmap_free_func val_free);

/include
    /ds
        hashmap.h
        history_system.h
        doubly_list.h
        lru_cache.h
        stack.h
        queue.h
        circular_list.h
        url_shortener.h
        round_robin.h
        rpn_calculator.h
        statistics.h
        next_next_practice.h
/src
    /ds
        hashmap.c
        history_system.c
        doubly_list.c
        lru_cache.c
        stack.c
        queue.c
        circular_list.c
        url_shortener.c
        round_robin.c
        rpn_calculator.c
        statistics.c
        next_next_practice.c

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

typedefやAPI名を全体で統一

未使用static関数は即削除

main.c以外ではmain関数は各テストごとにOKだが、プロダクト実装には含めない

Makefile・依存ファイルの自動検出も見直す

## 📦 Production-Ready Data Structures System アーキテクチャ仕様
（以下略、あなたの元文書そのままでOK）