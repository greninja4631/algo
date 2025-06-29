#ifndef METRICS_H
#define METRICS_H

#include <stddef.h>
#include <stdint.h>

typedef struct ds_stats {
    size_t total_elements;
    size_t memory_allocated;
    size_t operations_count;
    uint64_t creation_timestamp;
} ds_stats_t;

#endif // METRICS_H
