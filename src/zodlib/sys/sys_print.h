#pragma once

#include "sys_defines.h"
#include "sys_includes.h"

inline void sys_vfprint(FILE* stream, const char* fmt, va_list args) {
    std::vfprintf(stream, fmt, args);
}

inline void sys_fprint(FILE* stream, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    sys_vfprint(stream, fmt, args);
    va_end(args);
}

// internal — use the sys_print() macro instead, not called directly
#if defined(_DEBUG)
inline void sys_print_internal(const char* file, int line, const char* fmt,
                               ...) {
    va_list args;
    va_start(args, fmt);
    sys_vfprint(stdout, fmt, args);
    va_end(args);
    std::fprintf(stdout, " %s:%d\n", file, line);
}
#else
inline void sys_print_internal(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    sys_vfprint(stdout, fmt, args);
    va_end(args);
    std::fputc('\n', stdout);
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
    std::fprintf(stderr, "[%s] ", tag);
    va_list args;
    va_start(args, fmt);
    sys_vfprint(stderr, fmt, args);
    va_end(args);
    std::fprintf(stderr, " %s:%d\n", file, line);
}
#else
inline void sys_print_error_internal(const char* tag, const char* fmt, ...) {
    std::fprintf(stderr, "[%s] ", tag);
    va_list args;
    va_start(args, fmt);
    sys_vfprint(stderr, fmt, args);
    va_end(args);
    std::fputc('\n', stderr);
}
#endif

#if defined(_DEBUG)
#define sys_error(tag, fmt, ...) \
    sys_print_error_internal(tag, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define sys_error(tag, fmt, ...) \
    sys_print_error_internal(tag, fmt, ##__VA_ARGS__)
#endif
