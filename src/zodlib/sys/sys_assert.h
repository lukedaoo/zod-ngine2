#pragma once

#include "sys_includes.h"

#define assert_sizeof(type, size) \
    static_assert(sizeof(type) == (size), #type " must be " #size " bytes")

inline void assert_failed(const char* file, int line, const char* expr) {
    std::fprintf(stderr, "ASSERTION FAILED: %s, file %s, line %d\n", expr, file,
                 line);
    std::abort();
}

// x is dropped entirely in release - never pass an expression with side
// effects.
#if defined(_DEBUG)
#define zassert(x)                                 \
    {                                              \
        if (!(x)) {                                \
            assert_failed(__FILE__, __LINE__, #x); \
        }                                          \
    }
#else
#define zassert(x) ((void)0)
#endif

#ifdef assert
#undef assert
#endif
#define assert(x) zassert(x)

#if defined(_DEBUG)
#define strict_assert(x) zassert(x)
#else
#define strict_assert(x)                                        \
    {                                                           \
        if (!(x)) {                                             \
            std::fprintf(stderr, "ASSERTION FAILED: %s\n", #x); \
            std::abort();                                       \
        }                                                       \
    }
#endif
