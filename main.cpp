#include "zodlib/container/collections/zarray.h"
#include "zodlib/precompiled.h"

void func(const int* arr, int size) {
    for (int i = 0; i < size; ++i) {
        sys_std_print("%d", arr[i]);
    }
}

int main() {
    i8 a = 12;

    sys_std_print("%d", static_cast<int>(a));

    zArray<int, 10> arr;

    arr[0] = 12;
    sys_std_print("%d", static_cast<int>(arr[0]));

    zArray2D<int, 10, 10> arr2d;
    arr2d[0][0] = 13;

    sys_std_print("%d", static_cast<int>(arr2d[0][0]));

    int raw_arr[] = {1, 2, 3};

    func(ARRAY_DEF(raw_arr));

    i32* b = new i32(252);
    sys_std_print("%d", static_cast<i32>(*b));
    sys_mem_free(b);

#if defined(_DEBUG) && defined(_DEBUG_MEMORY)
    sys_mem_set_verbose_logging(true);
    i32* c = new i32(99);
    sys_std_print("%d", static_cast<i32>(*c));
    delete c;

    void* buf = sys_mem_alloc(64, MemTag::GENERAL);
    sys_mem_free(buf);

    int* single = new int(7);
    sys_mem_free(single);

    sys_mem_set_verbose_logging(false);

    MemReportEntry   entries[16];
    MemReportSummary summary = sys_mem_report(entries, 16);

    sys_std_print(
        "live=%d total_bytes=%d total_allocations=%d total_deallocations=%d",
        summary.count, summary.total_bytes, summary.total_allocations,
        summary.total_deallocations);
    for (int i = 0; i < summary.count; ++i) {
        sys_std_print("  %p size=%d tag=%d %s:%d", entries[i].ptr,
                      entries[i].size, (int)entries[i].tag, entries[i].file,
                      entries[i].line);
    }

    void* live_ptrs[10] = {};
    int   live_count    = 0;

    int cycle           = 0;
    while (cycle < 1000) {
        void* p = sys_mem_alloc(16, MemTag::GENERAL);
        if (cycle % 100 == 0 && live_count < 10) {
            live_ptrs[live_count++] = p;
        } else {
            sys_mem_free(p);
        }
        ++cycle;

        if (cycle % 250 == 0) {
            MemReportSummary s = sys_mem_report(nullptr, 0);
            sys_std_print(
                "cycle=%d live=%d total_bytes=%d "
                "total_bytes_per_tag[GENERAL]=%d "
                "total_bytes_per_tag[NEW]=%d total_allocations=%d "
                "total_deallocations=%d",
                cycle, s.count, s.total_bytes,
                s.total_bytes_per_tag[MemTag::GENERAL],
                s.total_bytes_per_tag[MemTag::NEW], s.total_allocations,
                s.total_deallocations);
        }
    }

    for (int j = 0; j < live_count; ++j) {
        sys_mem_free(live_ptrs[j]);
    }

    MemReportSummary final_summary = sys_mem_report(nullptr, 0);
    sys_std_print(
        "final: live=%d total_bytes=%d total_allocations=%d "
        "total_deallocations=%d",
        final_summary.count, final_summary.total_bytes,
        final_summary.total_allocations, final_summary.total_deallocations);

    sys_mem_set_verbose_logging(true);
    // Block usage: one tracked allocation backing several logical values,
    // carved up manually and freed all at once.
    int   block_size = sizeof(int) * 4;
    void* block      = sys_mem_alloc(block_size, MemTag::GENERAL);

    int* block_a     = (int*)block;
    int* block_b     = block_a + 1;
    int* block_c     = block_a + 2;
    int* block_d     = block_a + 3;

    *block_a         = 1;
    *block_b         = 2;
    *block_c         = 3;
    *block_d         = 4;

    sys_std_print("block: %d %d %d %d", *block_a, *block_b, *block_c, *block_d);

    sys_mem_free(block);  // one free for the whole block, not per value

#endif
    // zAreaManager: different types living in the same area, each constructed
    // through sys_mem_area_new<T> - no manual sizeof/alignof/placement-new.
    struct Vec3 {
        float x, y, z;
    };
    struct Entity {
        int  id;
        char name[16];
        Entity(int id_, const char* name_) : id(id_) {
            std::strncpy(name, name_, sizeof(name) - 1);
            name[sizeof(name) - 1] = '\0';
        }
    };

    zAreaHandle area = sys_mem_area_create(4096, MemTag::GENERAL);

    int*    n        = zAreaManager::put(area, 42);
    Vec3*   v        = zAreaManager::put(area, Vec3{1.0f, 2.0f, 3.0f});
    Entity* e        = zAreaManager::put(area, Entity(7, "player"));

    sys_std_print("area: n=%d v=(%f,%f,%f) e=%d:%s", *n, v->x, v->y, v->z,
                  e->id, e->name);

    sys_mem_area_destroy(area);

    return 0;
}
