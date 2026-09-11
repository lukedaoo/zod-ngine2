#include <UnitTest++/UnitTest++.h>

#include "zodlib/collections/zarray.h"

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
        zArray<int, 10> arr;
        arr.fill(1);
        arr.zero();
        for (int i = 0; i < arr.num(); ++i) {
            CHECK_EQUAL(arr[i], 0);
        }
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
        zArray2D<int, 2, 2> grid;
        grid[0][0] = 1;
        grid[1][1] = 1;
        grid.zero();
        for (int r = 0; r < grid.num_rows(); ++r) {
            for (int c = 0; c < grid.num_cols(); ++c) {
                CHECK_EQUAL(grid[r][c], 0);
            }
        }
    }
}
