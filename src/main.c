#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdarg.h>
#include <time.h>

#include "healthd.h"
#include "cpu_monitor.h"
#include "mem_monitor.h"
#include "thermal_monitor.h"
#include "watchdog.h"
#include "crash_logger.h"

#define POLL_INTERVAL_S 2

static volatile int running = 1;

static void sig_handler(int sig) {
    (void)sig;
    running = 0;
}

void healthd_log(LogLevel level, const char *fmt, ...) {
    const char *prefix[] = { LOG_INFO, LOG_WARN, LOG_ERROR };
    time_t now = time(NULL);
    char ts[20];
    strftime(ts, sizeof(ts), "%H:%M:%S", localtime(&now));

    printf("[%s] %s", ts, prefix[level]);
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    fflush(stdout);
}

int main(void) {
    signal(SIGINT,  sig_handler);
    signal(SIGTERM, sig_handler);

    healthd_log(LEVEL_INFO, "healthd starting...");

    cpu_monitor_init();
    watchdog_init(WATCHDOG_TIMEOUT_S);
    crash_logger_init("/var/log/healthd_crash.log");

    while (running) {
        CpuStats     cpu  = {0};
        MemStats     mem  = {0};
        ThermalStats thm  = {0};

        /* --- CPU --- */
        if (cpu_monitor_read(&cpu) == 0) {
            if (cpu.usage_percent >= CPU_WARN_THRESHOLD)
                healthd_log(LEVEL_WARN,  "CPU = %.0f%%", cpu.usage_percent);
            else
                healthd_log(LEVEL_INFO,  "CPU = %.0f%%", cpu.usage_percent);
        } else {
            crash_logger_record("cpu_monitor", "read failed");
        }

        /* --- Memory --- */
        if (mem_monitor_read(&mem) == 0) {
            if (mem.usage_percent >= MEM_WARN_THRESHOLD)
                healthd_log(LEVEL_WARN,  "Memory > 80%% (%.0f%%)", mem.usage_percent);
            else
                healthd_log(LEVEL_INFO,  "Memory = %.0f%%", mem.usage_percent);
        }

        /* --- Thermal --- */
        if (thermal_monitor_read(&thm) == 0) {
            if (thm.temp_celsius >= TEMP_WARN_CELSIUS)
                healthd_log(LEVEL_ERROR, "TEMP = %.1f°C — CRITICAL", thm.temp_celsius);
            else
                healthd_log(LEVEL_INFO,  "TEMP = %.1f°C", thm.temp_celsius);
        }

        watchdog_kick();
        sleep(POLL_INTERVAL_S);
    }

    healthd_log(LEVEL_INFO, "healthd stopping gracefully.");
    watchdog_destroy();
    return 0;
}