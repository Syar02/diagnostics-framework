#pragma once
#include <stdint.h>

typedef struct {
    float usage_percent;
} CpuStats;

int  cpu_monitor_init(void);
int  cpu_monitor_read(CpuStats *out);
void cpu_monitor_destroy(void);