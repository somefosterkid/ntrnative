#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_01_021FC4C4.h"

void ov01_021FC588(ResourceHeap *resHeap, u32 id);

ResourceHeap *ov01_021FC4C4(enum HeapID parentHeapID, enum HeapID heapID, u32 itemPoolSize, u32 itemCount);
void ov01_021FC520(ResourceHeap *resHeap);

ResourceHeap *ov01_021FC4C4(enum HeapID parentHeapID, enum HeapID heapID, u32 itemPoolSize, u32 itemCount) {
    u32 itemHeadersSize = sizeof(ResourceHeapItemHeader) * itemCount;
    u32 heapSize = itemPoolSize + itemHeadersSize + sizeof(ResourceHeap);

    BOOL heapCreated = Heap_Create(parentHeapID, heapID, heapSize);
    ((heapCreated == 1) ? (void)0 : GF_AssertFail());

    itemPoolSize = heapSize - itemPoolSize;
    ResourceHeap *resHeap = Heap_Alloc(heapID, itemPoolSize);

    ((resHeap != 0L) ? (void)0 : GF_AssertFail());
    memset(resHeap, 0, itemPoolSize);

    resHeap->parentHeapID = parentHeapID;
    resHeap->heapID = heapID;
    resHeap->itemCount = itemCount;
    resHeap->heapSize = heapSize;
    resHeap->itemHeadersSize = itemHeadersSize;
    resHeap->items = (ResourceHeapItemHeader *)(resHeap + 1);

    return resHeap;
}

void ov01_021FC520(ResourceHeap *resHeap) {
    u32 i = resHeap->itemCount;
    ResourceHeapItemHeader *iter = resHeap->items;

    while (i) {
        if (iter->data != 0L) {
            ov01_021FC588(resHeap, iter->id);
        }

        i--;
        iter++;
    }

    enum HeapID heapID = resHeap->heapID;

    Heap_Free(resHeap);
    Heap_Destroy(heapID);
}
