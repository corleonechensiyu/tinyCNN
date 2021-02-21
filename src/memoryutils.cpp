#include "memoryutils.h"

#include <stdint.h>
#include <stdlib.h>

static inline void** alignPointer(void** ptr, size_t alignment) {
    return (void**)((intptr_t)((unsigned char*)ptr + alignment - 1) & (~(alignment - 1)));
    //return (void**)((intptr_t)((unsigned char*)ptr + alignment - 1) & -alignment);
}

extern "C" void* MemoryAllocAlign(size_t size, size_t alignment)
{
    void** origin = (void**)malloc(size + sizeof(void*) + alignment);
    if (!origin) {
        return NULL;
    }
    void** aligned = alignPointer(origin + 1, alignment);
    aligned[-1] = origin;
    return aligned;
}
extern "C" void* MemoryCallocAlign(size_t size, size_t alignment)
{

    void** origin = (void**)calloc(size + sizeof(void*) + alignment, 1);
    if (!origin) {
        return NULL;
    }
    void** aligned = alignPointer(origin + 1, alignment);
    aligned[-1] = origin;
    return aligned;
}

extern "C" void MemoryFreeAlign(void* aligned)
{
    if (aligned) {
        void* origin = ((void**)aligned)[-1];
        free(origin);
    }
}