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









typedefやAPI名を全体で統一

未使用static関数は即削除

main.c以外ではmain関数は各テストごとにOKだが、プロダクト実装には含めない

Makefile・依存ファイルの自動検出も見直す







## 📦 Production-Ready Data Structures System アーキテクチャ仕様

### 1. ディレクトリ構成

```
.
├── include/
│   └── data_structures.h   # すべてのAPI宣言・型定義の中枢
├── src/
│   ├── doubly_list.c
│   ├── stack.c
│   ├── queue.c
│   ├── circular_list.c
│   ├── lru_cache.c
│   ├── hashmap.c
│   ├── history_system.c
│   ├── round_robin.c
│   ├── rpn_calculator.c
│   ├── url_shortener.c
│   └── util/
│       ├── logger.c
│       ├── memory.c
│       └── metrics.c
├── tests/
│   └── test_*.c   # 各構造体ごとに用意
├── build/
├── docs/
│   └── architecture.md  # ← このファイル
│   └── api.html
└── Makefile
``
src/main.c　（本番バイナリエントリが必要な場合のみ）

src/algo/ （アルゴリズム専用サブディレクトリ）

src/ds/ （データ構造本体専用サブディレクトリ）

src/util/ （本番用ユーティリティ or テスト補助専用）

---

### 2. 設計理念・ポリシー

* **APIは「ヘッダファースト」主義**
  実装・テスト・CIすべてが`include/data_structures.h`を唯一の“正”とする
* **全APIで戻り値型は`ds_error_t`で統一**（失敗時に詳細エラー返却）
* **構造体は`typedef struct ds_xxx ds_xxx_t;`で完全隠蔽型（OOPに近いC流儀）**
* **メモリアロケーション/ログ出力は依存性注入対応**

  * `ds_malloc`, `ds_free`, `ds_log`経由のみ許可（テストや本番で差し替え可）

---

### 3. 主なデータ構造・モジュール

| モジュール          | 機能         | API例                         |
| -------------- | ---------- | ---------------------------- |
| Stack          | 汎用LIFOスタック | `ds_stack_*`                 |
| Queue          | 汎用FIFOキュー  | `ds_queue_*`                 |
| Doubly List    | 双方向リスト     | `ds_doubly_list_*`           |
| Circular List  | 円環リスト      | `ds_circular_list_*`         |
| HashMap        | ハッシュマップ    | `ds_hashmap_*`               |
| LRU Cache      | LRUキャッシュ   | `ds_lru_cache_*`             |
| History System | Undo/Redo  | `ds_history_system_*`        |
| RPN Calculator | 逆ポーランド電卓   | `ds_rpn_calculator_*`        |
| Round Robin    | タイムシェアリング  | `ds_round_robin_scheduler_*` |
| URL Shortener  | URL短縮      | `ds_url_shortener_*`         |

---

### 4. エラーとログ

* **エラーは`ds_error_t`（enum）型で返却・API名は`DS_ERR_*`で統一**
* **エラー文字列化は`const char* ds_error_string(ds_error_t err)`で一元化**
* **ロギングは`ds_log(ds_log_level_t, ...)`に統一・テスト用モックも差し替え可**

---

### 5. テスト設計（CI自動化想定）

* すべてのテストは `tests/test_*.c` に1機能1ファイルで独立
* 全テストで「main.c」など本番エントリポイントは絶対混ぜない
  → テストは必ず`SRC_NOMAIN`を使ってmain排除でビルド
* `assert()`による失敗即検知方式（CI/CDでフェイルファスト）

---

### 6. Makefile設計

* 本番とテストで**コンパイル対象（main.cの有無）を分離**
* `test`ターゲットではmain.c排除済みのビルドを徹底（自動検出対応）
* テストごとに**実装ファイル側のAPI名/型と完全一致しているか**を確認

---

### 7. 拡張設計指針

* 新規データ構造追加時は

  * **include/data\_structures.hに必ず宣言追加**
  * 構造体/実装/テスト/API宣言全てで一貫性
* **メモリ/ログ/エラーすべて本アーキテクチャの統一APIを通す**

---

### 8. 典型的なコーディングパターン

#### (例) スタック取得API

```c
// include/data_structures.h
ds_error_t ds_stack_get_stats(const ds_stack_t* stack, ds_stats_t* stats);

// src/stack.c
ds_error_t ds_stack_get_stats(const ds_stack_t* stack, ds_stats_t* stats) {
    if (!stack || !stats) return DS_ERR_NULL_POINTER;
    stats->total_elements = stack->size;
    // ...
    return DS_SUCCESS;
}
```

* **宣言と実装が必ず一致すること**

---

### 9. 今後の開発・保守へのアドバイス

* **APIのズレはバグの温床。型・名前のブレを徹底排除**
* **CI/CDによる自動テスト・静的解析の継続推奨**
* **コメント・Doxygenによる自動ドキュメント生成で保守性アップ**
* **新しいDS追加や仕様拡張時も「ヘッダファースト原則」で進めること**
