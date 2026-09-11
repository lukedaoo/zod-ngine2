#include <iostream>  // std::cout

#include "zodlib/collections/zarray.h"
#include "zodlib/precompiled.h"

int main() {
    i8 a = 12;

    std::cout << static_cast<int>(a) << std::endl;

    zArray<int, 10> arr;

    arr[0] = 12;
    std::cout << static_cast<int>(arr[0]) << "\n";

    zArray2D<int, 10, 10> arr2d;
    arr2d[0][0] = 13;

    std::cout << static_cast<int>(arr2d[0][0]) << "\n";
    return 0;
}
