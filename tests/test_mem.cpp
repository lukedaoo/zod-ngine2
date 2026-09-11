#include <UnitTest++/UnitTest++.h>

#include "zodlib/precompiled.h"

#if defined(_DEBUG) && defined(_DEBUG_MEMORY)

namespace {
struct TestObj {
    int a;
    int b;
};
}  // namespace

SUITE(mem) {
    TEST(test_mem_alloc_free_delta) {
        MemReportSummary before = sys_mem_report(nullptr, 0);

        void* ptr               = sys_mem_alloc(64, MemTag::GENERAL);

        MemReportSummary during = sys_mem_report(nullptr, 0);
        CHECK_EQUAL(during.count, before.count + 1);
        CHECK_EQUAL(during.total_bytes, before.total_bytes + 64);
        CHECK_EQUAL(during.total_bytes_per_tag[MemTag::GENERAL],
                    before.total_bytes_per_tag[MemTag::GENERAL] + 64);

        sys_mem_free(ptr);

        MemReportSummary after = sys_mem_report(nullptr, 0);
        CHECK_EQUAL(after.count, before.count);
        CHECK_EQUAL(after.total_bytes, before.total_bytes);
    }

    TEST(test_mem_invariant_holds) {
        MemReportSummary summary = sys_mem_report(nullptr, 0);
        CHECK_EQUAL(summary.total_allocations - summary.total_deallocations,
                    (u64)summary.count);
    }

    TEST(test_mem_report_entries_populated) {
        MemReportSummary before = sys_mem_report(nullptr, 0);

        void* ptr               = sys_mem_alloc(32, MemTag::GENERAL);

        MemReportEntry   entries[8];
        MemReportSummary summary = sys_mem_report(entries, 8);

        bool found               = false;
        for (int i = 0; i < summary.count; ++i) {
            if (entries[i].ptr == ptr) {
                found = true;
                CHECK_EQUAL(entries[i].size, 32u);
                CHECK_EQUAL((int)entries[i].tag, (int)MemTag::GENERAL);
            }
        }
        CHECK(found);

        sys_mem_free(ptr);
        MemReportSummary after = sys_mem_report(nullptr, 0);
        CHECK_EQUAL(after.count, before.count);
    }

    TEST(test_mem_new_delete_tracked) {
        MemReportSummary before = sys_mem_report(nullptr, 0);

        TestObj* obj            = new TestObj();

        MemReportSummary during = sys_mem_report(nullptr, 0);
        CHECK_EQUAL(during.count, before.count + 1);

        delete obj;

        MemReportSummary after = sys_mem_report(nullptr, 0);
        CHECK_EQUAL(after.count, before.count);
    }

    TEST(test_mem_tagged_new_delete) {
        MemReportSummary before = sys_mem_report(nullptr, 0);

        TestObj* obj            = new (MemTag::NEW) TestObj();

        MemReportSummary during = sys_mem_report(nullptr, 0);
        CHECK_EQUAL(
            during.total_bytes_per_tag[MemTag::NEW],
            before.total_bytes_per_tag[MemTag::NEW] + (int)sizeof(TestObj));

        sys_mem_free(obj);

        MemReportSummary after = sys_mem_report(nullptr, 0);
        CHECK_EQUAL(after.count, before.count);
    }

    TEST(test_mem_new_array_delete_array_tracked) {
        MemReportSummary before = sys_mem_report(nullptr, 0);

        TestObj* arr            = new TestObj[4];

        MemReportSummary during = sys_mem_report(nullptr, 0);
        CHECK_EQUAL(during.count, before.count + 1);

        delete[] arr;

        MemReportSummary after = sys_mem_report(nullptr, 0);
        CHECK_EQUAL(after.count, before.count);
    }
}

#endif
