#include "../../precompiled.h"

#if defined(_DEBUG) && defined(_DEBUG_MEMORY)

MemRecord  zMemTracker::s_records[MAX_TRACKED_ALLOCS] = {};
std::mutex zMemTracker::s_mutex{};
u64        zMemTracker::s_total_allocations{0};
u64        zMemTracker::s_total_deallocations{0};
bool       zMemTracker::s_verbose_logging{false};

void* zMemTracker::alloc(int size, MemTag tag, const char* file, int line) {
    void* ptr = std::malloc(size);

    std::lock_guard<std::mutex> lock(s_mutex);
    s_total_allocations++;

    bool tracked = false;
    for (usize i = 0; i < MAX_TRACKED_ALLOCS; ++i) {
        if (s_records[i].used) {
            continue;
        }
        s_records[i] = {.ptr  = ptr,
                        .size = (u32)size,
                        .tag  = tag,
                        .file = file,
                        .line = line,
                        .used = true};
        tracked      = true;
        break;
    }

    if (!tracked) {
        static bool warned = false;
        if (!warned) {
            sys_std_error(
                "zMemTracker",
                "MAX_TRACKED_ALLOCS exceeded, tracking disabled for new "
                "allocations");
            warned = true;
        }
    }

    if (s_verbose_logging) {
        sys_fprint(stdout, "ALLOC %p size=%d tag=%d %s:%d\n", ptr, size, tag,
                   file, line);
    }

    return ptr;
}

void zMemTracker::free(void* ptr, const char* file, int line) {
    if (!ptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(s_mutex);
    s_total_deallocations++;

    for (usize i = 0; i < MAX_TRACKED_ALLOCS; ++i) {
        if (s_records[i].used && s_records[i].ptr == ptr) {
            if (s_verbose_logging) {
                sys_fprint(stdout, "FREE %p size=%u tag=%d %s:%d\n", ptr,
                           s_records[i].size, s_records[i].tag, file, line);
            }
            s_records[i] = {};
            break;
        }
    }

    std::free(ptr);
}

MemReportSummary zMemTracker::report(MemReportEntry* out_entries,
                                     int             capacity) {
    std::lock_guard<std::mutex> lock(s_mutex);

    MemReportSummary summary    = {};
    summary.total_allocations   = s_total_allocations;
    summary.total_deallocations = s_total_deallocations;

    for (usize i = 0; i < MAX_TRACKED_ALLOCS; ++i) {
        if (!s_records[i].used) {
            continue;
        }

        summary.total_bytes += s_records[i].size;
        summary.total_bytes_per_tag[s_records[i].tag] += s_records[i].size;

        if (out_entries && summary.count < capacity) {
            out_entries[summary.count] = {.ptr  = s_records[i].ptr,
                                          .size = s_records[i].size,
                                          .tag  = s_records[i].tag,
                                          .file = s_records[i].file,
                                          .line = s_records[i].line};
        }
        summary.count++;
    }

    return summary;
}

void zMemTracker::set_verbose_logging(bool enabled) {
    std::lock_guard<std::mutex> lock(s_mutex);
    s_verbose_logging = enabled;
}

bool zMemTracker::detect_leaks() {
    MemReportSummary probe = report(nullptr, 0);
    if (probe.count == 0) {
        return false;
    }

    MemReportEntry   entries[256];
    MemReportSummary summary = report(entries, 256);

    for (int i = 0; i < summary.count; ++i) {
        sys_fprint(stderr, "[zMemTracker] LEAK: %u bytes at %s:%d\n",
                   entries[i].size, entries[i].file, entries[i].line);
    }
    if (summary.count < probe.count) {
        sys_std_error("zMemTracker", "... and %d more",
                      probe.count - summary.count);
    }

    return true;
}

#endif
