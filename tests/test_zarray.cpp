#include <UnitTest++/UnitTest++.h>

#include "zodlib/container/collections/zarray.h"

SUITE(zarray) {
    TEST(test_zarray) {
        zArray<int, 10> arr;
        arr[0] = 12;
        CHECK_EQUAL(arr[0], 12);
    }

    TEST(test_zarray_num) {
        zArray<int, 10> arr;
        CHECK_EQUAL(arr.num(), 10);
    }

    TEST(test_zarray_byte_size) {
        zArray<int, 10> arr;
        CHECK_EQUAL(arr.byte_size(), (int)(10 * sizeof(int)));
    }

    TEST(test_zarray_zero) {
        zArray<unsigned char, 10> arr;
        arr.fill(1);
        arr.zero();
        for (int i = 0; i < arr.num(); ++i) {
            CHECK_EQUAL(arr[i], (unsigned char)ZERO_MEMORY);
        }
    }

    // zero() must poison memory with ZERO_MEMORY (default: 0xCD) in debug
    // builds, not just clear it, so uninitialized reads are easy to spot in a
    // debugger.
    // @see: zodlib/sys/sys_defines.h
    TEST(test_zarray_zero_debug_pattern) {
#if defined(_DEBUG)
        zArray<unsigned char, 10> arr;
        arr.fill(0);
        arr.zero();
        for (int i = 0; i < arr.num(); ++i) {
            CHECK_EQUAL(arr[i], (unsigned char)ZERO_MEMORY);
        }
#endif
    }

    TEST(test_zarray_fill) {
        zArray<char, 10> arr;
        arr.fill(7);
        for (int i = 0; i < arr.num(); ++i) {
            CHECK_EQUAL(arr[i], 7);
        }
    }

    TEST(test_zarray_independent_indices) {
        zArray<int, 3> arr;
        arr[0] = 1;
        arr[1] = 2;
        arr[2] = 3;
        CHECK_EQUAL(arr[0], 1);
        CHECK_EQUAL(arr[1], 2);
        CHECK_EQUAL(arr[2], 3);
    }

    TEST(test_zarray_update) {
        zArray<int, 3> arr;
        arr[0] = 1;
        arr[1] = 2;
        arr[2] = 3;
        CHECK_EQUAL(arr[0], 1);
        CHECK_EQUAL(arr[1], 2);
        CHECK_EQUAL(arr[2], 3);

        arr[0] = 4;
        arr[1] = 5;
        arr[2] = 6;
        CHECK_EQUAL(arr[0], 4);
        CHECK_EQUAL(arr[1], 5);
        CHECK_EQUAL(arr[2], 6);
    }

    TEST(test_zarray2d_dimensions) {
        zArray2D<int, 3, 4> grid;
        CHECK_EQUAL(grid.num_rows(), 3);
        CHECK_EQUAL(grid.num_cols(), 4);
    }

    TEST(test_zarray2d_byte_size) {
        zArray2D<int, 3, 4> grid;
        CHECK_EQUAL(grid.byte_size(), (int)(3 * 4 * sizeof(int)));
    }

    TEST(test_zarray2d_read_write) {
        zArray2D<int, 3, 3> grid;
        grid[0][0] = 1;
        grid[1][2] = 5;
        grid[2][1] = 9;
        CHECK_EQUAL(grid[0][0], 1);
        CHECK_EQUAL(grid[1][2], 5);
        CHECK_EQUAL(grid[2][1], 9);
    }

    TEST(test_zarray2d_zero) {
        zArray2D<unsigned char, 2, 2> grid;
        grid[0][0] = 1;
        grid[1][1] = 1;
        grid.zero();
        for (int r = 0; r < grid.num_rows(); ++r) {
            for (int c = 0; c < grid.num_cols(); ++c) {
                CHECK_EQUAL(grid[r][c], (unsigned char)ZERO_MEMORY);
            }
        }
    }
}

SUITE(zread_only_array) {
    TEST(test_zread_only_array_from_pointer) {
        int                    raw[3] = {1, 2, 3};
        zReadOnlyArray<int, 3> view(raw);
        CHECK_EQUAL(view.num(), 3);
        CHECK_EQUAL(view[0], 1);
        CHECK_EQUAL(view[1], 2);
        CHECK_EQUAL(view[2], 3);
    }

    TEST(test_zread_only_array_from_zarray) {
        zArray<int, 3> arr;
        arr[0] = 4;
        arr[1] = 5;
        arr[2] = 6;

        zReadOnlyArray<int, 3> view(arr);
        CHECK_EQUAL(view.num(), 3);
        CHECK_EQUAL(view[0], 4);
        CHECK_EQUAL(view[1], 5);
        CHECK_EQUAL(view[2], 6);
    }

    TEST(test_zread_only_array_reflects_source_mutation) {
        zArray<int, 2> arr;
        arr[0] = 1;
        arr[1] = 2;

        zReadOnlyArray<int, 2> view(arr);
        CHECK_EQUAL(view[0], 1);

        arr[0] = 9;
        CHECK_EQUAL(view[0], 9);
    }

    TEST(test_zarray_to_readonly) {
        zArray<int, 3> arr;
        arr[0] = 7;
        arr[1] = 8;
        arr[2] = 9;

        auto view = arr.to_readonly();
        CHECK_EQUAL(view.num(), 3);
        CHECK_EQUAL(view[0], 7);
        CHECK_EQUAL(view[1], 8);
        CHECK_EQUAL(view[2], 9);

        arr[0] = 1;
        CHECK_EQUAL(view[0], 1);
    }
}
