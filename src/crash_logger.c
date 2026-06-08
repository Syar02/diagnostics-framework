#include "crash_logger.h"
#include "healthd.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

static char log_path[256] = "/var/log/healthd_crash.log";

void crash_logger_init(const char *path) {
    if (path) strncpy(log_path, path, sizeof(log_path) - 1);
}

void crash_logger_record(const char *component, const char *reason) {
    FILE *f = fopen(log_path, "a");
    if (!f) return;

    time_t now = time(NULL);
    char tsbuf[32];
    strftime(tsbuf, sizeof(tsbuf), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fprintf(f, "[%s] CRASH component=%s reason=%s\n", tsbuf, component, reason);
    fclose(f);

    healthd_log(LEVEL_ERROR, "CRASH: %s — %s", component, reason);
}

void crash_logger_dump(void) {
    FILE *f = fopen(log_path, "r");
    if (!f) { printf("No crash log found.\n"); return; }
    char line[256];
    while (fgets(line, sizeof(line), f)) printf("%s", line);
    fclose(f);
}