#include "../../include/data_structures.h"
#include "../../include/ds/statistics.h"  // ds_statistics_t型/API宣言
#include <stddef.h>

// ---- 内部ヘルパー関数 ----
static int sum(const int *data, size_t size) {
    int s = 0;
    for (size_t i = 0; i < size; i++) s += data[i];
    return s;
}

static int min(const int *data, size_t size) {
    int m = data[0];
    for (size_t i = 1; i < size; i++) if (data[i] < m) m = data[i];
    return m;
}

static int max(const int *data, size_t size) {
    int m = data[0];
    for (size_t i = 1; i < size; i++) if (data[i] > m) m = data[i];
    return m;
}

static double average(const int *data, size_t size) {
    if (size == 0) return 0.0;
    return (double)sum(data, size) / size;
}

static double median(const int *data, size_t size) {
    if (size == 0) return 0.0;
    int *tmp = (int*)ds_malloc(sizeof(int) * size);
    if (!tmp) {
        ds_log(DS_LOG_ERROR, "[statistics] メモリ確保失敗（median計算）\n");
        return 0.0;
    }
    for (size_t i = 0; i < size; i++) tmp[i] = data[i];
    // シンプル挿入ソート
    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = i + 1; j < size; j++) {
            if (tmp[i] > tmp[j]) {
                int t = tmp[i]; tmp[i] = tmp[j]; tmp[j] = t;
            }
        }
    }
    double med;
    if (size % 2) med = tmp[size/2];
    else med = (tmp[size/2 - 1] + tmp[size/2]) / 2.0;
    ds_free(tmp);
    return med;
}

static int mode(const int *data, size_t size) {
    int mode = data[0], max_count = 1;
    for (size_t i = 0; i < size; i++) {
        int count = 1;
        for (size_t j = i + 1; j < size; j++) {
            if (data[i] == data[j]) count++;
        }
        if (count > max_count) {
            max_count = count;
            mode = data[i];
        }
    }
    return mode;
}

// ---- API本体 ----
ds_statistics_t ds_calculate_statistics(const int *data, size_t size) {
    ds_statistics_t stats = {0};
    // 不正入力は安全に終了（ログ出力のみ）
    if (size == 0 || !data) {
        ds_log(DS_LOG_ERROR, "[statistics] 入力データが無効です（size=%zu, data=%p）\n", size, (void*)data);
        return stats;
    }
    stats.sum     = sum(data, size);
    stats.min     = min(data, size);
    stats.max     = max(data, size);
    stats.average = average(data, size);
    stats.median  = median(data, size);
    stats.mode    = mode(data, size);
    return stats;
}
