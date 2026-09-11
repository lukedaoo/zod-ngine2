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

struct zArea;
class zAreaManager;
