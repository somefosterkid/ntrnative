#include "global.h"

#include "constants/heap.h"

#include "heap.h"

void ov00_021E77CC(void **allocStartPtr, void **alignedPtr, int size, enum HeapID heapID);

void ov00_021E77CC(void **allocStartPtr, void **alignedPtr, int size, enum HeapID heapID) {
    *allocStartPtr = Heap_Alloc(heapID, size + 32);
    MI_CpuClear8(*allocStartPtr, size + 32);
    *alignedPtr = ((void *)(((u32)(*allocStartPtr) + (32 - 1)) / (32) * (32)));
}
