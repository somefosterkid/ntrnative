#include "global.h"

#include "constants/heap.h"

#include "field/hblank_system_priv.h"

#include "heap.h"

HBlankSystem *HBlankSystem_New(enum HeapID heapID) {
    HBlankSystem *hBlankSystem;

    hBlankSystem = Heap_Alloc(heapID, sizeof(HBlankSystem));
    ov01_021FB55C(hBlankSystem);
    return hBlankSystem;
}
