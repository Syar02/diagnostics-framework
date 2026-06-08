#pragma once

void crash_logger_init(const char *log_path);
void crash_logger_record(const char *component, const char *reason);
void crash_logger_dump(void);