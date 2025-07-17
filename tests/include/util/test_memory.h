#ifndef DS_TEST_UTIL_METRICS_H
#define DS_TEST_UTIL_METRICS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file    tests/include/ds/test_util_metrics.h
 * @brief   メトリクス機能のユニットテスト宣言（実装は.cのみ）
 * @note    ODR違反や多重定義リスクを避けるため、ここでは宣言のみ
 */

/**
 * @brief メトリクスAPIの基本動作検証
 * @test   CIの自動検出用関数名 (test__<module>_<case>)
 */
void test__metrics_basic(void);

#ifdef __cplusplus
}
#endif

#endif /* DS_TEST_UTIL_METRICS_H */
