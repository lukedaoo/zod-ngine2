#pragma once

#include "../sys_mem.h"
// Only be enabled while debugging
// In the release build, memory tracking is disabled
#if defined(_DEBUG) && defined(_DEBUG_MEMORY)

// @todo(sys_mem):
// 1. Have options to enable/disable memory tracking at runtime
// 2. Have options to track all allocations or just tracked upto
// @MAX_TRACKED_ALLOCS. Potential solution: Use a circular buffer
// 3. Have options to enable/disable verbose logging
#ifndef MAX_TRACKED_ALLOCS
#define MAX_TRACKED_ALLOCS 8192 * 10
#endif

struct MemReportEntry {
    void*       ptr;
    u32         size;
    MemTag      tag;
    const char* file;
    int         line;
};

struct MemReportSummary {
    int count;
    u64 total_bytes;
    u64 total_bytes_per_tag[NUM_MEM_TAGS];
    u64 total_allocations;
    u64 total_deallocations;
};

struct MemRecord {
    void*       ptr;
    u32         size;
    MemTag      tag;
    const char* file;
    int         line;
    bool        used;
};

class zMemTracker {
private:
    static MemRecord  s_records[MAX_TRACKED_ALLOCS];
    static std::mutex s_mutex;
    static u64        s_total_allocations;
    static u64        s_total_deallocations;
    static bool       s_verbose_logging;
public:
    static void* alloc(int size, MemTag tag, const char* file, int line);
    static void  free(void* ptr, const char* file, int line);
    static MemReportSummary report(MemReportEntry* out_entries, int capacity);
    static void             set_verbose_logging(bool enabled);
    // Call at a point where nothing should still be allocated (program
    // exit, level unload, test teardown). Prints each leaked allocation's
    // size and the file:line it came from. Returns true if any leaks were
    // found.
    static bool             detect_leaks();
};

inline MemReportSummary sys_mem_report(MemReportEntry* out_entries,
                                       int             capacity) {
    return zMemTracker::report(out_entries, capacity);
}

inline void sys_mem_set_verbose_logging(bool enabled) {
    zMemTracker::set_verbose_logging(enabled);
}

#endif
