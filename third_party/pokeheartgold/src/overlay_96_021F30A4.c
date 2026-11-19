#include "global.h"

#include "heap.h"

void *ov96_021F30A4(enum HeapID arg0);

void *ov96_021F30A4(enum HeapID heapID) {
    void *dest;

    dest = Heap_Alloc(heapID, 0x84U);
    MI_CpuFill8(dest, 0U, 0x84U);
    return dest;
}
