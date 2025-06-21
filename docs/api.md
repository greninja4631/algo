# Data Structures API Documentation

**Version:** 1.0.0  
**Date:** 2025-05-28  
**Author:** シリコンバレー標準設計テンプレート


以下の概念でエラー修正希望です。


私のCプロジェクト（Dockerビルド）で発生しているビルドエラーについて、  
**1つ1つのエラーをソフトウェア工学・設計責務の観点から徹底的に分解・解説し、**  
必ず**「どこにどの責務・API宣言があるべきか」**を整理してもらい、  
**最終的に100％ビルドが通る（Docker+make testでOK）状態の修正版コードを、.h/.c丸ごとコピペできる形で**  
1つずつ提示してください。

- エラーの該当ログ・Dockerfile・Makefile・ディレクトリ構成をすべて提示します。
- コードは「責務の分離」を必ず守り、型やenumやAPIの再定義・未定義エラーを絶対に起こさないようにしてください。
- 「どのモジュールがどこに依存してよいか」を明示し、依存方向の逆流や多重定義を起こさない形を必須とします。
- 修正提案は部分パッチではなく「全体をそのまま上書き可能な完成版コード」でお願いします。
- 原因分析から「設計責務の教科書的説明→解決策→フルコード例」までセットで1つずつお願いします。

【備考】
- push/popなどの操作は必ずstack.cのみで実装してください。
- enum, typedef, structの本体実装は.cのみに限定し、.hには宣言だけを残してください。
- make testがエラー無く通るまでの最短ルートを重視してください。





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