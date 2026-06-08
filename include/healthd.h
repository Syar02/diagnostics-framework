#pragma once
#include <stdint.h>
#include <time.h>

#define LOG_INFO    "[INFO] "
#define LOG_WARN    "[WARNING] "
#define LOG_ERROR   "[ERROR] "

#define CPU_WARN_THRESHOLD  80
#define MEM_WARN_THRESHOLD  80
#define TEMP_WARN_CELSIUS   75
#define WATCHDOG_TIMEOUT_S  5

typedef enum {
    LEVEL_INFO = 0,
    LEVEL_WARN,
    LEVEL_ERROR
} LogLevel;

void healthd_log(LogLevel level, const char *fmt, ...);