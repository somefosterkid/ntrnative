#include "global.h"

#include "constants/heap.h"

#include "sys_task.h"
#include "overlay_01_021FC4C4.h"

struct ResourceHeapItemHeader;
void *Heap_Alloc(u32 heapID, u32 size);
void *Heap_AllocAtEnd(u32 heapID, u32 size);
void Heap_Free(void *ptr);

void *ov01_021FC5FC(ResourceHeap *resHeap, u32 size, BOOL allocAtEnd);
void ov01_021FC61C(void *ptr);

void *ov01_021FC5FC(ResourceHeap *resHeap, u32 size, BOOL allocAtEnd) {
    void *data;

    if (allocAtEnd == 0) {
        data = Heap_Alloc(resHeap->heapID, size);
    } else {
        data = Heap_AllocAtEnd(resHeap->heapID, size);
    }

    ((data != 0L) ? (void)0 : GF_AssertFail());
    return data;
}

void ov01_021FC61C(void *ptr) {
    Heap_Free(ptr);
}
