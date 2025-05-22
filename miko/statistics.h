#ifndef STATISTICS_H
#define STATISTICS_H

// ------- 公開API用（再利用・テストしたいものだけ宣言） -------

typedef struct {
    int sum;
    int min;
    int max;
    double average;
    double median;
    int mode;
} Statistics;

typedef enum {
    STAT_SUCCESS = 0,
    STAT_MEM_ALLOC_FAIL = 1,
    STAT_INVALID_INPUT = 2
} ErrorCode;

// 必要な関数プロトタイプ（API公開）
int sum(const int *data, int size);
int min(const int *data, int size);
int max(const int *data, int size);
double average(const int *data, int size);
double median(const int *data, int size);
int mode(const int *data, int size);
Statistics calculate_statistics(const int *data, int size);

#endif // STATISTICS_H