#pragma once

#include "sys_defines.h"
#include "sys_includes.h"

// internal — use the sys_print() macro instead, not called directly
#if defined(_DEBUG)
inline void sys_print_internal(const char* file, int line, const char* fmt,
                               ...) {
    char    buf[MAX_PRINT_MESSAGE_CHARS];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    std::fprintf(stdout, "%s %s:%d\n", buf, file, line);
}
#else
inline void sys_print_internal(const char* fmt, ...) {
    char    buf[MAX_PRINT_MESSAGE_CHARS];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    std::fputs(buf, stdout);
}
#endif

#if defined(_DEBUG)
#define sys_print(fmt, ...) \
    sys_print_internal(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define sys_print(fmt, ...) sys_print_internal(fmt, ##__VA_ARGS__)
#endif

// internal — use the sys_error() macro instead, not called directly
#if defined(_DEBUG)
inline void sys_print_error_internal(const char* tag, const char* file,
                                     int line, const char* fmt, ...) {
    char    buf[MAX_PRINT_MESSAGE_CHARS];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    std::fprintf(stderr, "[%s] %s %s:%d\n", tag, buf, file, line);
}
#else
inline void sys_print_error_internal(const char* tag, const char* fmt, ...) {
    char    buf[MAX_PRINT_MESSAGE_CHARS];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    std::fprintf(stderr, "[%s] %s", tag, buf);
}
#endif

#if defined(_DEBUG)
#define sys_error(tag, fmt, ...) \
    sys_print_error_internal(tag, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define sys_error(tag, fmt, ...) \
    sys_print_error_internal(tag, fmt, ##__VA_ARGS__)
#endif
