#include "../precompiled.h"

void* sys_mem_alloc_full(const int size, const MemTag tag, const char* file,
                         const int line) {
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

void* operator new(std::size_t size) {
    return sys_mem_alloc(size, MemTag::NEW);
}

void* operator new[](std::size_t size) {
    return sys_mem_alloc(size, MemTag::NEW);
}

void* operator new(std::size_t size, MemTag tag) {
    return sys_mem_alloc(size, tag);
}

void* operator new[](std::size_t size, MemTag tag) {
    return sys_mem_alloc(size, tag);
}

void operator delete(void* ptr) noexcept { sys_mem_free(ptr); }

void operator delete(void* ptr, std::size_t) noexcept { sys_mem_free(ptr); }

void operator delete[](void* ptr) noexcept { sys_mem_free(ptr); }

void operator delete[](void* ptr, std::size_t) noexcept { sys_mem_free(ptr); }

void operator delete(void* ptr, MemTag) noexcept { sys_mem_free(ptr); }

void operator delete[](void* ptr, MemTag) noexcept { sys_mem_free(ptr); }

#endif
