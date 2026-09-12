#pragma once

#include "../sys_mem.h"

#ifndef MAX_AREAS
#define MAX_AREAS 32
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
    static zAreaHandle create(int size, MemTag tag, const char* file = nullptr,
                              int line = 0);
    static void*       alloc(zAreaHandle handle, int size, int alignment);
    static void        reset(zAreaHandle handle);
    static void        destroy(zAreaHandle handle, const char* file = nullptr,
                               int line = 0);
    static bool        is_valid(zAreaHandle handle);
    // return the total capacity
    static int         get_capacity(zAreaHandle handle);
    // return the number of bytes allocated
    static int         get_size(zAreaHandle handle);
    // return the number of bytes free
    static int         get_free(zAreaHandle handle);

#pragma push_macro("new")
#undef new
    template <class _type_>
    static _type_* put(zAreaHandle handle, const _type_& data) {
        void* ptr = alloc(handle, sizeof(_type_), alignof(_type_));
        strict_assert(ptr);
        return new (ptr) _type_(data);
    }
#pragma pop_macro("new")
};

inline zAreaHandle sys_mem_area_create_full(int size, MemTag tag,
                                            const char* file, int line) {
    return zAreaManager::create(size, tag, file, line);
}
