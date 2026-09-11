#pragma once

#include "sys_included.h"

#define assert_sizeof(type, size) \
    static_assert(sizeof(type) == (size), #type " must be " #size " bytes")

inline void assert_failed(const char* file, int line, const char* expr) {
    std::fprintf(stderr, "ASSERTION FAILED: %s, file %s, line %d\n", expr, file,
                 line);
    std::abort();
}

#if defined(_DEBUG)
#define zassert(x)                                \
    {                                             \
        if (!(x)) {                               \
            assert_failed(__FILE__, __LINE__, #x); \
        }                                         \
    }
#else
#define zassert(x) ((void)0)
#endif

#ifdef assert
#undef assert
#endif
#define assert(x) zassert(x)
