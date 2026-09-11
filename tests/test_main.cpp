#include <UnitTest++/TestReporterStdout.h>
#include <UnitTest++/UnitTest++.h>

TEST(Sanity) { CHECK_EQUAL(4, 2 + 2); }

int main(int argc, char** argv) {
    const char*                  suite_name = (argc > 1) ? argv[1] : nullptr;

    UnitTest::TestReporterStdout reporter;
    UnitTest::TestRunner         runner(reporter);

    return runner.RunTestsIf(UnitTest::Test::GetTestList(), suite_name,
                             UnitTest::True(), 0);
}
