#include "../../precompiled.h"

void* sys_mem_alloc_full(const int size, const MemTag tag, const char* file,
                         const int line) {
    assert(size >= 0);
#if defined(_DEBUG) && defined(_DEBUG_MEMORY)
    return zMemTracker::alloc(size, tag, file, line);
#else
    (void)tag;
    (void)file;
    (void)line;
    return std::malloc(size);
#endif
}

void sys_mem_free_full(void* ptr, const char* file, const int line) {
#if defined(_DEBUG) && defined(_DEBUG_MEMORY)
    zMemTracker::free(ptr, file, line);
#else
    (void)file;
    (void)line;
    std::free(ptr);
#endif
}

#if defined(_DEBUG) && defined(_DEBUG_MEMORY)

#pragma push_macro("new")
#undef new

void* operator new(std::size_t size) {
    return sys_mem_alloc(size, MemTag::NEW);
}

void* operator new[](std::size_t size) {
    return sys_mem_alloc(size, MemTag::NEW);
}

void* operator new(std::size_t size, const char* file, int line) {
    return sys_mem_alloc_full(size, MemTag::NEW, file, line);
}

void* operator new[](std::size_t size, const char* file, int line) {
    return sys_mem_alloc_full(size, MemTag::NEW, file, line);
}

static void warn_delete_no_location(void* ptr) {
    sys_print(
        "sys_mem: `delete` used on %p - no call site info, prefer "
        "sys_mem_free() instead",
        ptr);
}

void operator delete(void* ptr) noexcept {
    warn_delete_no_location(ptr);
    sys_mem_free(ptr);
}

void operator delete(void* ptr, std::size_t) noexcept {
    warn_delete_no_location(ptr);
    sys_mem_free(ptr);
}

void operator delete[](void* ptr) noexcept {
    warn_delete_no_location(ptr);
    sys_mem_free(ptr);
}

void operator delete[](void* ptr, std::size_t) noexcept {
    warn_delete_no_location(ptr);
    sys_mem_free(ptr);
}

void operator delete(void* ptr, const char* file, int line) noexcept {
    sys_mem_free_full(ptr, file, line);
}

void operator delete[](void* ptr, const char* file, int line) noexcept {
    sys_mem_free_full(ptr, file, line);
}

#pragma pop_macro("new")

#endif
