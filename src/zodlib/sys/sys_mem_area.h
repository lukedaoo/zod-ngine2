#pragma once

#include "sys_mem.h"

#define INVALID_AREA (-1)

#ifndef MAX_AREAS
#define MAX_AREAS 32
#endif

typedef int zAreaHandle;
struct zArea;

zArea sys_mem_area_create_full(int size, MemTag tag, const char* file,
                               int line);

#if defined(_DEBUG) && defined(_DEBUG_MEMORY)
#define sys_mem_area_create(size, tag) \
    sys_mem_area_create_full(size, tag, __FILE__, __LINE__)
#else
#define sys_mem_area_create(size, tag) \
    sys_mem_area_create_full(size, tag, nullptr, 0)
#endif

class zAreaManager {
private:
    struct Slot {
        void*      block;
        int        capacity;
        int        offset;
        std::mutex mutex;
        bool       used;
    };
    // @todo: auto grow
    static Slot s_areas[MAX_AREAS];
public:
    static zAreaHandle create(int size, MemTag tag, const char* file, int line);
    static void*       alloc(zAreaHandle handle, int size, int alignment);
    static void        reset(zAreaHandle handle);
    static void        destroy(zAreaHandle handle);
    static bool        is_valid(zAreaHandle handle);
    // return the total capacity
    static int         get_capacity(zAreaHandle handle);
    // return the number of bytes allocated
    static int         get_size(zAreaHandle handle);
    // return the number of bytes free
    static int         get_free(zAreaHandle handle);
};

struct zArea {
    zAreaHandle handle;

    void reset() { zAreaManager::reset(handle); }
    void destroy() { zAreaManager::destroy(handle); }
    bool is_valid() const { return zAreaManager::is_valid(handle); }

    template <class _type_>
    _type_* put(const _type_& data) {
        void* ptr =
            zAreaManager::alloc(handle, sizeof(_type_), alignof(_type_));
        strict_assert(ptr);
        return new (ptr) _type_(data);
    }
};

inline zArea sys_mem_area_create_full(int size, MemTag tag, const char* file,
                                      int line) {
    return zArea{zAreaManager::create(size, tag, file, line)};
}
