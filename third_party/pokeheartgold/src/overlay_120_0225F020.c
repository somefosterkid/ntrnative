#include "global.h"

#include "heap.h"
#include "overlay_120.h"

Ov120Tile *ov120_0225F020(enum HeapID heapID) {
    Ov120Tile *v0;

    v0 = Heap_Alloc(heapID, sizeof(Ov120Tile));
    memset(v0, 0, sizeof(Ov120Tile));
    return v0;
}
