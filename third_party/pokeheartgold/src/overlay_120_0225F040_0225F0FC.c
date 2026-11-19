#include "global.h"

#include "heap.h"
#include "overlay_120.h"

Ov120Wipe *ov120_0225F0FC(enum HeapID heapID) {
    Ov120Wipe *v0;

    v0 = Heap_Alloc(heapID, sizeof(Ov120Wipe));
    memset(v0, 0, sizeof(Ov120Wipe));
    return v0;
}
