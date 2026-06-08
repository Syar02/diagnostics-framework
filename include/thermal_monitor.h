#pragma once

typedef struct {
    float temp_celsius;
    char  zone[64];
} ThermalStats;

int thermal_monitor_read(ThermalStats *out);