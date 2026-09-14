#include <UnitTest++/UnitTest++.h>

#include "zodlib/precompiled.h"

namespace {
struct Vec3 {
    float x, y, z;
};
}  // namespace

SUITE(mem_area) {
    TEST(test_mem_area_put_and_read) {
        zAreaHandle area = sys_mem_area_create(256, MemTag::GENERAL);

        int* n = zAreaManager::put(area, 42);
        CHECK_EQUAL(*n, 42);

        sys_mem_area_destroy(area);
    }

    TEST(test_mem_area_multiple_types) {
        zAreaHandle area = sys_mem_area_create(256, MemTag::GENERAL);

        int*  n = zAreaManager::put(area, 7);
        Vec3* v = zAreaManager::put(area, Vec3{1.0f, 2.0f, 3.0f});

        CHECK_EQUAL(*n, 7);
        CHECK_EQUAL(v->x, 1.0f);
        CHECK_EQUAL(v->y, 2.0f);
        CHECK_EQUAL(v->z, 3.0f);

        sys_mem_area_destroy(area);
    }

    TEST(test_mem_area_put_is_aligned) {
        zAreaHandle area = sys_mem_area_create(256, MemTag::GENERAL);

        zAreaManager::put(area, (char)1);
        Vec3* v = zAreaManager::put(area, Vec3{1.0f, 2.0f, 3.0f});

        CHECK_EQUAL((usize)v % alignof(Vec3), (usize)0);

        sys_mem_area_destroy(area);
    }

    TEST(test_mem_area_is_valid) {
        zAreaHandle area = sys_mem_area_create(256, MemTag::GENERAL);
        CHECK(zAreaManager::is_valid(area));

        sys_mem_area_destroy(area);
        CHECK(!zAreaManager::is_valid(area));
    }

    TEST(test_mem_area_capacity_size_free) {
        zAreaHandle area = sys_mem_area_create(256, MemTag::GENERAL);

        CHECK_EQUAL(zAreaManager::get_capacity(area), 256);
        CHECK_EQUAL(zAreaManager::get_size(area), 0);
        CHECK_EQUAL(zAreaManager::get_free(area), 256);

        zAreaManager::alloc(area, 64, 1);

        CHECK_EQUAL(zAreaManager::get_capacity(area), 256);
        CHECK_EQUAL(zAreaManager::get_size(area), 64);
        CHECK_EQUAL(zAreaManager::get_free(area), 192);

        sys_mem_area_destroy(area);
    }

    TEST(test_mem_area_capacity_size_free_after_reset) {
        zAreaHandle area = sys_mem_area_create(256, MemTag::GENERAL);

        zAreaManager::alloc(area, 64, 1);
        zAreaManager::reset(area);

        CHECK_EQUAL(zAreaManager::get_size(area), 0);
        CHECK_EQUAL(zAreaManager::get_free(area), 256);

        sys_mem_area_destroy(area);
    }

    TEST(test_mem_area_capacity_size_free_invalid_handle) {
        CHECK_EQUAL(zAreaManager::get_capacity(INVALID_AREA), -1);
        CHECK_EQUAL(zAreaManager::get_size(INVALID_AREA), -1);
        CHECK_EQUAL(zAreaManager::get_free(INVALID_AREA), -1);
    }

    TEST(test_mem_area_reset_resets_offset_counter) {
        zAreaHandle area = sys_mem_area_create(256, MemTag::GENERAL);

        zAreaManager::put(area, 1);
        int size_before_reset = zAreaManager::get_size(area);

        zAreaManager::reset(area);
        zAreaManager::put(area, 2);
        int size_after_reset = zAreaManager::get_size(area);

        CHECK_EQUAL(size_before_reset, size_after_reset);

        sys_mem_area_destroy(area);
    }

#if defined(_DEBUG) && defined(_DEBUG_MEMORY)
    TEST(test_mem_area_backing_block_is_tracked) {
        MemReportSummary before = sys_mem_report(nullptr, 0);

        zAreaHandle       area    = sys_mem_area_create(128, MemTag::GENERAL);
        MemReportSummary during = sys_mem_report(nullptr, 0);
        CHECK_EQUAL(during.count, before.count + 1);

        sys_mem_area_destroy(area);
        MemReportSummary after = sys_mem_report(nullptr, 0);
        CHECK_EQUAL(after.count, before.count);
    }
#endif
}
