typedef struct {
    int sum, min, max, mode;
    float average, median;
} Statistics;
Statistics calculate_statistics(const int *data, int size);
