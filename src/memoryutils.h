#ifndef MEMORYUTILS_H_
#define MEMORYUTILS_H_
#include <stdio.h>
#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ALIGN_DEFAULT 64

PUBLIC void* MemoryAllocAlign(size_t size, size_t align);

PUBLIC void* MemoryCallocAlign(size_t size, size_t align);

PUBLIC void MemoryFreeAlign(void* mem);

#ifdef __cplusplus
}
#endif

#endif // !MEMORYUTILS_H_

