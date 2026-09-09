#pragma once

#define assert_sizeof(type, size) \
    static_assert(sizeof(type) == (size), #type " must be " #size " bytes")
