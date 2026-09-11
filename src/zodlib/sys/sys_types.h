#pragma once

#include <cstddef>

// clang-format off
//
// Int types
//
typedef unsigned char      byte;   // 8 bits
typedef unsigned short     word;   // 16 bits
typedef unsigned int       dword;  // 32 bits

typedef signed char        i8;
typedef signed short       i16;
typedef signed int         i32;
typedef signed long long   i64;

typedef i8                 int8;
typedef i16                int16;
typedef i32                int32;
typedef i64                int64;

typedef byte               u8;
typedef word               u16;
typedef dword              u32;
typedef unsigned long long u64;

typedef u8                 uint8;
typedef u16                uint16;
typedef u32                uint32;
typedef u64                uint64;

typedef u8                 _1byte;
typedef u16                _2byte;
typedef u32                _4byte;
typedef u64                _8byte;

typedef __SIZE_TYPE__      usize;

//
// Float types
//
typedef float              f32;
typedef f32                float32;
typedef f32                real32;

typedef double             f64;
typedef f64                float64;
typedef f64                real64;

// clang-format on

#include "../sys/sys_assert.h"

assert_sizeof(bool, 1);
assert_sizeof(char, 1);
assert_sizeof(short, 2);
assert_sizeof(int, 4);
assert_sizeof(long long, 8);

assert_sizeof(byte, 1);
assert_sizeof(word, 2);
assert_sizeof(dword, 4);

assert_sizeof(i8, 1);
assert_sizeof(i16, 2);
assert_sizeof(i32, 4);
assert_sizeof(i64, 8);

assert_sizeof(u8, 1);
assert_sizeof(u16, 2);
assert_sizeof(u32, 4);
assert_sizeof(u64, 8);

assert_sizeof(_1byte, 1);
assert_sizeof(_2byte, 2);
assert_sizeof(_4byte, 4);
assert_sizeof(_8byte, 8);

assert_sizeof(usize, sizeof(void*));

assert_sizeof(f32, 4);
assert_sizeof(f64, 8);

template <class T>
inline T Max(T x, T y) {
    return x > y ? x : y;
}

template <class T>
inline T Min(T x, T y) {
    return x < y ? x : y;
}

template <class T>
inline T Clamp(T x, T min, T max) {
    return Max(min, Min(x, max));
}

#ifndef BIT
#define BIT(num) (1ULL << (num))
#endif

#ifndef NUMBITS
#define NUMBITS(_type_) (sizeof(_type_) * 8)
#endif
