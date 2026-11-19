#include "global.h"
#include "constants/heap.h"
#include "overlay_01_021FC4C4.h"


ResourceHeapItemHeader *ov01_021FC624(ResourceHeap *resHeap, u32 id);
ResourceHeapItemHeader *ov01_021FC644(ResourceHeap *resHeap);

ResourceHeapItemHeader *ov01_021FC624(ResourceHeap *resHeap, u32 id) {
    u32 i = resHeap->itemCount;
    ResourceHeapItemHeader *iter = resHeap->items;

    while (i) {
        if (iter->data != 0L && iter->id == id) {
            return iter;
        }

        iter++;
        i--;
    }

    return 0L;
}

ResourceHeapItemHeader *ov01_021FC644(ResourceHeap *resHeap) {
    u32 i = resHeap->itemCount;
    ResourceHeapItemHeader *iter = resHeap->items;

    while (i) {
        if (iter->data == 0L) {
            return iter;
        }

        iter++;
        i--;
    }

    return 0L;
}
