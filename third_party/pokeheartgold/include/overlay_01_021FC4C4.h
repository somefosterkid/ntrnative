#ifndef POKEHEARTGOLD_OVERLAY_01_021FC4C4_H
#define POKEHEARTGOLD_OVERLAY_01_021FC4C4_H

#include "global.h"

#include "constants/heap.h"

// The field effects' resource heap (ov01_021FC4C4): `itemCount` slots of (data, id), filled by id.
typedef struct ResourceHeapItemHeader {
    void *data;
    u32 id;
} ResourceHeapItemHeader;

typedef struct ResourceHeap {
    enum HeapID parentHeapID;
    enum HeapID heapID;
    u32 itemCount;
    u32 heapSize;
    u32 itemHeadersSize;
    ResourceHeapItemHeader *items;
} ResourceHeap;

ResourceHeap *ov01_021FC4C4(enum HeapID parentHeapID, enum HeapID heapID, u32 itemPoolSize, u32 itemCount);
void ov01_021FC588(ResourceHeap *resHeap, u32 id);
void *ov01_021FC5A4(ResourceHeap *resHeap, u32 id);
BOOL ov01_021FC5B8(ResourceHeap *resHeap, u32 id);
ResourceHeapItemHeader *ov01_021FC624(ResourceHeap *resHeap, u32 id);

#endif // POKEHEARTGOLD_OVERLAY_01_021FC4C4_H
