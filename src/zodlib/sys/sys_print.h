#pragma once

#include "sys_defines.h"
#include "sys_includes.h"
#include "sys_types.h"

inline void sys_vfprint(FILE* stream, const char* fmt, va_list args) {
    std::vfprintf(stream, fmt, args);
}

inline void sys_fprint(FILE* stream, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    sys_vfprint(stream, fmt, args);
    va_end(args);
}

// for a plain string with a known length — skips vfprintf's format-string
// parsing
inline void sys_write(FILE* stream, const char* str, usize len) {
    std::fwrite(str, 1, len, stream);
}

#if defined(_DEBUG)
inline void sys_print_internal(FILE* stream, const char* file, int line,
                               const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    sys_vfprint(stream, fmt, args);
    va_end(args);
    std::fprintf(stream, " %s:%d\n", file, line);
}
#else
inline void sys_print_internal(FILE* stream, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    sys_vfprint(stream, fmt, args);
    va_end(args);
    std::fputc('\n', stream);
}
#endif

#if defined(_DEBUG)
#define sys_std_print(fmt, ...) \
    sys_print_internal(stdout, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define sys_std_print(fmt, ...) sys_print_internal(stdout, fmt, ##__VA_ARGS__)
#endif

#if defined(_DEBUG)
inline void sys_print_error_internal(FILE*       stream = stderr,
                                     const char* tag    = "ERROR",
                                     const char* file = nullptr, int line = 0,
                                     const char* fmt = nullptr, ...) {
    std::fprintf(stream, "[%s] ", tag);
    va_list args;
    va_start(args, fmt);
    sys_vfprint(stream, fmt, args);
    va_end(args);
    std::fprintf(stream, " %s:%d\n", file, line);
}
#else
inline void sys_print_error_internal(FILE*       stream = stderr,
                                     const char* tag = "ERROR", const char* fmt,
                                     ...) {
    std::fprintf(stream, "[%s] ", tag);
    va_list args;
    va_start(args, fmt);
    sys_vfprint(stream, fmt, args);
    va_end(args);
    std::fputc('\n', stream);
}
#endif

#if defined(_DEBUG)
#define sys_std_error(tag, fmt, ...)                               \
    sys_print_error_internal(stderr, tag, __FILE__, __LINE__, fmt, \
                             ##__VA_ARGS__)
#else
#define sys_std_error(tag, fmt, ...) \
    sys_print_error_internal(stderr, tag, fmt, ##__VA_ARGS__)
#endif
