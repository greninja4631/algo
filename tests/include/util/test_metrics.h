/**
 * @file  tests/include/util/test_metrics.h
 * @brief Metrics モジュール用・テスト関数宣言だけを置くヘッダ
 *
 * - **実装は書かない** ‥‥ tests/util/test_metrics.c にのみ実装を書く  
 * - Include-Guard はガイドライン準拠の `DS_TEST_UTIL_METRICS_H` に統一  
 * - 古いガード名 `TEST_UTIL_TEST_METRICS_H` も定義しておき、
 *   既存コードがあってもビルドが切れないように互換層を残す
 */

#ifndef DS_TEST_UTIL_METRICS_H          /* 新ガード */
#define DS_TEST_UTIL_METRICS_H
#define TEST_UTIL_TEST_METRICS_H        /* 旧ガード互換（中身なし） */

#ifdef __cplusplus
extern "C" {
#endif

/* ──────────────────────────────────────────
 * 宣言専用：ここに実装を書かないこと！
 * ────────────────────────────────────────── */
void test__metrics_basic(void);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* DS_TEST_UTIL_METRICS_H */
