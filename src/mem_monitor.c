#include "mem_monitor.h"
#include <stdio.h>
#include <string.h>

int mem_monitor_read(MemStats *out) {
    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) return -1;

    char key[64];
    unsigned long val;
    uint64_t mem_total = 0, mem_available = 0;

    while (fscanf(f, "%63s %lu kB\n", key, &val) == 2) {
        if (!strcmp(key, "MemTotal:"))     mem_total     = val;
        if (!strcmp(key, "MemAvailable:")) mem_available = val;
    }
    fclose(f);

    out->total_kb       = mem_total;
    out->used_kb        = mem_total - mem_available;
    out->usage_percent  = (mem_total == 0) ? 0.0f
                        : (float)out->used_kb / (float)mem_total * 100.0f;
    return 0;
}