#include "cpu_monitor.h"
#include <stdio.h>
#include <string.h>

static unsigned long long prev_idle = 0, prev_total = 0;

int cpu_monitor_init(void) {
    CpuStats tmp;
    return cpu_monitor_read(&tmp); /* prime the counters */
}

int cpu_monitor_read(CpuStats *out) {
    FILE *f = fopen("/proc/stat", "r");
    if (!f) return -1;

    unsigned long long user, nice, sys, idle, iowait, irq, softirq;
    fscanf(f, "cpu %llu %llu %llu %llu %llu %llu %llu",
           &user, &nice, &sys, &idle, &iowait, &irq, &softirq);
    fclose(f);

    unsigned long long total = user + nice + sys + idle + iowait + irq + softirq;
    unsigned long long dtotal = total - prev_total;
    unsigned long long didle  = idle  - prev_idle;

    out->usage_percent = (dtotal == 0) ? 0.0f
                       : (float)(dtotal - didle) / (float)dtotal * 100.0f;

    prev_total = total;
    prev_idle  = idle;
    return 0;
}

void cpu_monitor_destroy(void) {}