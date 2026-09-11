#include "../../precompiled.h"

zAreaManager::Slot zAreaManager::s_areas[MAX_AREAS] = {};

zAreaHandle zAreaManager::create(int size, MemTag tag, const char* file,
                                 int line) {
    for (int i = 0; i < MAX_AREAS; ++i) {
        if (s_areas[i].used) {
            continue;
        }

        s_areas[i].block    = sys_mem_alloc_full(size, tag, file, line);
        s_areas[i].capacity = size;
        s_areas[i].offset   = 0;
        s_areas[i].used     = true;

        return i;
    }

    return INVALID_AREA;
}

void* zAreaManager::alloc(zAreaHandle handle, int size, int alignment) {
    assert((unsigned)handle < MAX_AREAS);
    Slot& slot = s_areas[handle];
    assert(slot.used);

    std::lock_guard<std::mutex> lock(slot.mutex);

    int aligned_offset = (slot.offset + alignment - 1) & ~(alignment - 1);
    if (aligned_offset + size > slot.capacity) {
        // @roburstness: should it move to the next area?
        return nullptr;
    }

    void* ptr   = (char*)slot.block + aligned_offset;
    slot.offset = aligned_offset + size;

    return ptr;
}

void zAreaManager::reset(zAreaHandle handle) {
    assert((unsigned)handle < MAX_AREAS);
    Slot& slot = s_areas[handle];
    assert(slot.used);

    std::lock_guard<std::mutex> lock(slot.mutex);
    slot.offset = 0;
}

void zAreaManager::destroy(zAreaHandle handle, const char* file, int line) {
    assert((unsigned)handle < MAX_AREAS);
    Slot& slot = s_areas[handle];
    assert(slot.used);

    sys_mem_free_full(slot.block, file, line);
    slot.block    = nullptr;
    slot.capacity = 0;
    slot.offset   = 0;
    slot.used     = false;
}

bool zAreaManager::is_valid(zAreaHandle handle) {
    if ((unsigned)handle >= MAX_AREAS) {
        return false;
    }
    return s_areas[handle].used;
}

int zAreaManager::get_capacity(zAreaHandle handle) {
    return is_valid(handle) ? s_areas[handle].capacity : -1;
}

int zAreaManager::get_size(zAreaHandle handle) {
    return is_valid(handle) ? s_areas[handle].offset : -1;
}

int zAreaManager::get_free(zAreaHandle handle) {
    return is_valid(handle) ? s_areas[handle].capacity - s_areas[handle].offset
                            : -1;
}
