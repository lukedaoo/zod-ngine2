#include <UnitTest++/UnitTest++.h>

#include "zodlib/precompiled.h"

namespace {
struct Vec3 {
    float x, y, z;
};
}  // namespace

SUITE(mem_area) {
    TEST(test_mem_area_put_and_read) {
        zArea area = sys_mem_area_create(256, MemTag::GENERAL);

        int* n     = area.put(42);
        CHECK_EQUAL(*n, 42);

        area.destroy();
    }

    TEST(test_mem_area_multiple_types) {
        zArea area = sys_mem_area_create(256, MemTag::GENERAL);

        int*  n    = area.put(7);
        Vec3* v    = area.put(Vec3{1.0f, 2.0f, 3.0f});

        CHECK_EQUAL(*n, 7);
        CHECK_EQUAL(v->x, 1.0f);
        CHECK_EQUAL(v->y, 2.0f);
        CHECK_EQUAL(v->z, 3.0f);

        area.destroy();
    }

    TEST(test_mem_area_put_is_aligned) {
        zArea area = sys_mem_area_create(256, MemTag::GENERAL);

        area.put(
            (char)1);  // odd-sized put to knock the bump pointer off alignment
        Vec3* v = area.put(Vec3{1.0f, 2.0f, 3.0f});

        CHECK_EQUAL((usize)v % alignof(Vec3), (usize)0);

        area.destroy();
    }

    TEST(test_mem_area_is_valid) {
        zArea area = sys_mem_area_create(256, MemTag::GENERAL);
        CHECK(area.is_valid());

        area.destroy();
        CHECK(!area.is_valid());
    }

    TEST(test_mem_area_capacity_size_free) {
        zArea area = sys_mem_area_create(256, MemTag::GENERAL);

        CHECK_EQUAL(zAreaManager::get_capacity(area.handle), 256);
        CHECK_EQUAL(zAreaManager::get_size(area.handle), 0);
        CHECK_EQUAL(zAreaManager::get_free(area.handle), 256);

        zAreaManager::alloc(area.handle, 64, 1);

        CHECK_EQUAL(zAreaManager::get_capacity(area.handle), 256);
        CHECK_EQUAL(zAreaManager::get_size(area.handle), 64);
        CHECK_EQUAL(zAreaManager::get_free(area.handle), 192);

        area.destroy();
    }

    TEST(test_mem_area_capacity_size_free_after_reset) {
        zArea area = sys_mem_area_create(256, MemTag::GENERAL);

        zAreaManager::alloc(area.handle, 64, 1);
        area.reset();

        CHECK_EQUAL(zAreaManager::get_size(area.handle), 0);
        CHECK_EQUAL(zAreaManager::get_free(area.handle), 256);

        area.destroy();
    }

    TEST(test_mem_area_capacity_size_free_invalid_handle) {
        CHECK_EQUAL(zAreaManager::get_capacity(INVALID_AREA), -1);
        CHECK_EQUAL(zAreaManager::get_size(INVALID_AREA), -1);
        CHECK_EQUAL(zAreaManager::get_free(INVALID_AREA), -1);
    }

    TEST(test_mem_area_reset_reuses_offset) {
        zArea area  = sys_mem_area_create(256, MemTag::GENERAL);

        void* first = area.put(1);
        area.reset();
        void* second = area.put(2);

        CHECK_EQUAL(first, second);

        area.destroy();
    }

#if defined(_DEBUG) && defined(_DEBUG_MEMORY)
    TEST(test_mem_area_backing_block_is_tracked) {
        MemReportSummary before = sys_mem_report(nullptr, 0);

        zArea            area   = sys_mem_area_create(128, MemTag::GENERAL);
        MemReportSummary during = sys_mem_report(nullptr, 0);
        CHECK_EQUAL(during.count, before.count + 1);

        area.destroy();
        MemReportSummary after = sys_mem_report(nullptr, 0);
        CHECK_EQUAL(after.count, before.count);
    }
#endif
}
