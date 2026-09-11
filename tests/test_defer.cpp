#include <UnitTest++/UnitTest++.h>

#include "zodlib/precompiled.h"

SUITE(defer) {
    TEST(test_defer_runs_on_scope_exit) {
        bool ran = false;
        {
            defer { ran = true; };
            CHECK_EQUAL(ran, false);
        }
        CHECK_EQUAL(ran, true);
    }

    TEST(test_defer_runs_on_early_return) {
        bool ran         = false;
        auto scoped_call = [&]() {
            defer { ran = true; };
            return;
        };
        scoped_call();
        CHECK_EQUAL(ran, true);
    }

    TEST(test_defer_runs_in_declaration_order) {
        int order[2] = {0, 0};
        int step     = 0;
        {
            defer { order[1] = ++step; };
            defer { order[0] = ++step; };
        }
        CHECK_EQUAL(order[0], 1);
        CHECK_EQUAL(order[1], 2);
    }

    TEST(test_defer_nested_scopes) {
        int order[2] = {0, 0};
        int step     = 0;
        {
            defer { order[0] = ++step; };
            {
                defer { order[1] = ++step; };
                CHECK_EQUAL(order[0], 0);
                CHECK_EQUAL(order[1], 0);
            }
            // inner scope ended: inner defer already ran, outer hasn't
            CHECK_EQUAL(order[1], 1);
            CHECK_EQUAL(order[0], 0);
        }
        // outer scope ended: outer defer ran too
        CHECK_EQUAL(order[1], 1);
        CHECK_EQUAL(order[0], 2);
    }
}
