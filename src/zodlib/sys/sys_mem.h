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

template <class _type_>
void sys_mem_free_full(_type_* ptr, const char* file, int line) {
    if (!ptr) {
        return;
    }
    ptr->~_type_();
    sys_mem_free_full((void*)ptr, file, line);
}

#if defined(_DEBUG) && defined(_DEBUG_MEMORY)
#define sys_mem_alloc(size, tag) \
    sys_mem_alloc_full(size, tag, __FILE__, __LINE__)
#define sys_mem_free(ptr) sys_mem_free_full(ptr, __FILE__, __LINE__)

void* operator new(std::size_t size);
void* operator new[](std::size_t size);
void* operator new(std::size_t size, const char* file, int line);
void* operator new[](std::size_t size, const char* file, int line);

void operator delete(void* ptr) noexcept;
void operator delete(void* ptr, std::size_t) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete[](void* ptr, std::size_t) noexcept;
void operator delete(void* ptr, const char* file, int line) noexcept;
void operator delete[](void* ptr, const char* file, int line) noexcept;

#define DEBUG_MEMORY_NEW_KEYWORD_TEMPLATE new (__FILE__, __LINE__)
#undef new
#define new DEBUG_MEMORY_NEW_KEYWORD_TEMPLATE

#else
#define sys_mem_alloc(size, tag) sys_mem_alloc_full(size, tag)
#define sys_mem_free(ptr)        sys_mem_free_full(ptr)
#endif

class zAreaManager;
