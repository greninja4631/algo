# Data Structures API Documentation

**Version:** 1.0.0  
**Date:** 2025-05-28  
**Author:** シリコンバレー標準設計テンプレート

---

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