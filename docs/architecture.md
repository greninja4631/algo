# プロジェクトアーキテクチャ設計ドキュメント

## 1. ディレクトリ・レイヤー構成

```text
src/
├── stack.c
├── queue.c
├── doubly_list.c
├── rpn_calculator.c
├── round_robin.c
├── history_system.c
├── util/
│   ├── logger.c
│   ├── metrics.c
│   └── memory.c
include/
├── data_structures.h
└── config.h
tests/
├── test_stack.c
├── test_queue.c
├── test_doubly_list.c
├── test_rpn_calculator.c
├── test_round_robin.c
├── test_history_system.c
└── test_util.c
docs/
├── api.md
└── architecture.md


	•	src/: すべての実装本体を格納。データ構造/アルゴリズム/ユーティリティごとに.cファイル分離。

	•	src/util/: ロガー・メトリクス・メモリ管理等「横断的な共通機能」を分離し、責務分離・依存性注入を実現。

	•	include/: APIヘッダー（.h）専用。全ての外部公開APIは必ずここに宣言。内部実装は絶対に隠蔽。

	•	tests/: 各モジュールごとの単体テスト。main()以外のAPIはすべてテスト駆動。
    
	•	docs/: アーキテクチャ設計/API仕様書などドキュメント専用。