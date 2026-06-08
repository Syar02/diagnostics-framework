cmake_minimum_required(VERSION 3.16)
project(healthd VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

include_directories(include)

set(SOURCES
    src/main.c
    src/cpu_monitor.c
    src/mem_monitor.c
    src/thermal_monitor.c
    src/watchdog.c
    src/crash_logger.c
)

add_executable(healthd ${SOURCES})
target_compile_definitions(healthd PRIVATE _GNU_SOURCE)

install(TARGETS healthd RUNTIME DESTINATION /usr/local/bin)