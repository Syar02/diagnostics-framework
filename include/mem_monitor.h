#pragma once
#include <stdint.h>

typedef struct {
    uint64_t total_kb;
    uint64_t used_kb;
    float    usage_percent;
} MemStats;

int  mem_monitor_read(MemStats *out);