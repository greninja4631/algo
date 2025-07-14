#ifndef DS_TEST_UTIL_METRICS_H
#define DS_TEST_UTIL_METRICS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ──────────────────────────────────────────
 * “宣言専用”テストヘッダ
 * 実装は .c ファイルにのみ置き、重複シンボルを防止
 * ────────────────────────────────────────── */
void test__metrics_basic(void);

#ifdef __cplusplus
}
#endif

#endif /* DS_TEST_UTIL_METRICS_H */