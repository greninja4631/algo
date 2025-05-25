#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdio.h>

// ログ標準化マクロ（Cloud/CIログにも流用可）
#define LOG_INFO(...)   fprintf(stdout,  "[INFO]  " __VA_ARGS__)
#define LOG_ERROR(...)  fprintf(stderr, "[ERROR] " __VA_ARGS__)

/** 
 * @file statistics.h
 * @brief 統計計算API (抽象データ型/インターフェース分離) 
 * 
 * [設計理由]
 * - main関数非依存のAPI定義。再利用・テスト容易性・ドキュメント自動生成(Doxygen)対応。
 * - 構造体/関数/エラーコードは全て明示宣言、内部実装隠蔽。
 * - main以外は全てテスト可能な純粋関数として設計。
 */

/** 統計量の集計構造体 (抽象データ型) */
typedef struct {
    int sum;        ///< 合計値
    int min;        ///< 最小値
    int max;        ///< 最大値
    double average; ///< 平均値
    double median;  ///< 中央値
    int mode;       ///< 最頻値
} Statistics;

/** エラーコード分類 (型安全/enum) */
typedef enum {
    STAT_SUCCESS = 0,           ///< 正常終了
    STAT_MEM_ALLOC_FAIL = 1,    ///< メモリ確保失敗
    STAT_INVALID_INPUT = 2      ///< 不正な入力
} ErrorCode;

/** 公開API (全てユニットテスト可能) */
int sum(const int *data, int size);
int min(const int *data, int size);
int max(const int *data, int size);
double average(const int *data, int size);
double median(const int *data, int size);
int mode(const int *data, int size);
Statistics calculate_statistics(const int *data, int size);

#endif // STATISTICS_H