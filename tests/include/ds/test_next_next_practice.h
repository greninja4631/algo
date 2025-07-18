/* tests/include/ds/test_next_next_practice.h
 * ------------------------------------------------------------
 * ds_next_next_practice モジュール専用 ― テスト用宣言ヘッダ
 * 2025-07 ガイドライン（Opaque 型／マイクロアーキテクチャ）準拠
 * ------------------------------------------------------------ */
#ifndef DS_TEST_NEXT_NEXT_PRACTICE_H
#define DS_TEST_NEXT_NEXT_PRACTICE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 基本動作テスト
 */
void test__next_next_practice_basic(void);

/**
 * @brief NULL 安全／異常系テスト
 */
void test__next_next_practice_edge_cases(void);

#ifdef __cplusplus
}
#endif

#endif /* DS_TEST_NEXT_NEXT_PRACTICE_H */
/*
 * tests/include/ds/test_next_next_practice.h
 * ------------------------------------------------------------
 *  ds_next_next_practice ― ユニットテスト宣言ヘッダ
 *  2025-07 完全統一ガイドライン準拠
 * ------------------------------------------------------------
 */
#ifndef DS_TEST_NEXT_NEXT_PRACTICE_H
#define DS_TEST_NEXT_NEXT_PRACTICE_H

#ifdef __cplusplus
extern "C" {
#endif

/** 基本動作テスト */
void test__next_next_practice_basic(void);

/** NULL-safe／異常系テスト */
void test__next_next_practice_edge_cases(void);

#ifdef __cplusplus
}
#endif

#endif /* DS_TEST_NEXT_NEXT_PRACTICE_H */