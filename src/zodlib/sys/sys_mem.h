#pragma once

#include "sys_types.h"
enum MemTag : u8 {
#define MEM_TAG(name) name,
#include "sys_alloc_tags.h"
#undef MEM_TAG
    NUM_MEM_TAGS
};

void* sys_mem_alloc_full(const int size, const MemTag tag = MemTag::GENERAL,
                         const char* file = nullptr, const int line = 0);
void  sys_mem_free_full(void* ptr, const char* file = nullptr,
                        const int line = 0);

#if defined(_DEBUG) && defined(_DEBUG_MEMORY)

#ifndef MAX_TRACKED_ALLOCS
#define MAX_TRACKED_ALLOCS 8192
#endif

struct MemRecord {
    void*       ptr;
    u32         size;
    MemTag      tag;
    const char* file;
    int         line;
    bool        used;
};

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

#define sys_mem_alloc(size, tag) \
    sys_mem_alloc_full(size, tag, __FILE__, __LINE__)
#define sys_mem_free(ptr) sys_mem_free_full(ptr, __FILE__, __LINE__)

void* operator new(std::size_t size);
void* operator new[](std::size_t size);
void* operator new(std::size_t size, MemTag tag);
void* operator new[](std::size_t size, MemTag tag);

void operator delete(void* ptr) noexcept;
void operator delete(void* ptr, std::size_t) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete[](void* ptr, std::size_t) noexcept;
// Only invoked if a constructor throws inside `new (tag) Foo()` - not a normal
// delete path.
void operator delete(void* ptr, MemTag) noexcept;
void operator delete[](void* ptr, MemTag) noexcept;

#else

#define sys_mem_alloc(size, tag) sys_mem_alloc_full(size, tag)
#define sys_mem_free(ptr)        sys_mem_free_full(ptr)

#endif

inline void* sys_mem_alloc_full(const int size, const MemTag tag,
                                const char* file, const int line) {
#if defined(_DEBUG) && defined(_DEBUG_MEMORY)
    return zMemTracker::alloc(size, tag, file, line);
#else
    (void)tag;
    (void)file;
    (void)line;
    return std::malloc(size);
#endif
}

inline void sys_mem_free_full(void* ptr, const char* file, const int line) {
#if defined(_DEBUG) && defined(_DEBUG_MEMORY)
    zMemTracker::free(ptr, file, line);
#else
    (void)file;
    (void)line;
    std::free(ptr);
#endif
}
