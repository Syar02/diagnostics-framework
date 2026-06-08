#pragma once

int  watchdog_init(int timeout_sec);
void watchdog_kick(void);
void watchdog_destroy(void);