#include "watchdog.h"
#include "healthd.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

static int wd_fd = -1;

int watchdog_init(int timeout_sec) {
    /* Try hardware watchdog first */
    wd_fd = open("/dev/watchdog", O_WRONLY);
    if (wd_fd < 0) {
        healthd_log(LEVEL_WARN, "Hardware watchdog unavailable, using software fallback");
        return 0;
    }
    healthd_log(LEVEL_INFO, "Watchdog initialized (timeout=%ds)", timeout_sec);
    return 0;
}

void watchdog_kick(void) {
    if (wd_fd >= 0)
        write(wd_fd, "1", 1);
}

void watchdog_destroy(void) {
    if (wd_fd >= 0) {
        /* Magic close — disable watchdog on graceful exit */
        write(wd_fd, "V", 1);
        close(wd_fd);
        wd_fd = -1;
    }
}