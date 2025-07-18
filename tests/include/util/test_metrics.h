#ifndef DS_TEST_UTIL_METRICS_H
#define DS_TEST_UTIL_METRICS_H
#define TEST_UTIL_TEST_METRICS_H  /* 古い互換ガード名（中身なし） */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file    tests/include/util/test_metrics.h
 * @brief   Metricsモジュール用 テスト関数宣言
 * @note
 * - 実装は tests/util/test_metrics.c のみ
 * - 本ファイルには実装を書かないこと！
 * - テスト関数名は test__metrics_<case>() 形式で統一
 */

/* 基本機能テスト */
void test__metrics_basic(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DS_TEST_UTIL_METRICS_H */