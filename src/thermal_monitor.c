#include "thermal_monitor.h"
#include <stdio.h>
#include <string.h>

/* Linux thermal sysfs path */
#define THERMAL_PATH "/sys/class/thermal/thermal_zone0/temp"

int thermal_monitor_read(ThermalStats *out) {
    FILE *f = fopen(THERMAL_PATH, "r");
    if (!f) {
        /* Fallback: return dummy value if no thermal zone */
        out->temp_celsius = -1.0f;
        snprintf(out->zone, sizeof(out->zone), "unavailable");
        return -1;
    }
    int raw = 0;
    fscanf(f, "%d", &raw);
    fclose(f);

    out->temp_celsius = (float)raw / 1000.0f;
    snprintf(out->zone, sizeof(out->zone), "thermal_zone0");
    return 0;
}