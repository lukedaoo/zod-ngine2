#include <UnitTest++/UnitTest++.h>

TEST(Sanity) {
    CHECK_EQUAL(4, 2 + 2);
}

int main() {
    return UnitTest::RunAllTests();
}
