#include "global.h"

#include "heap.h"

#include "overlay_2/overlay_02_02248728.h"

FieldSystemUnkSub120 *ov02_0224F864(enum HeapID heapID) {
    FieldSystemUnkSub120 *unkSub120 = Heap_Alloc(heapID, sizeof(FieldSystemUnkSub120));

    MI_CpuFill8(unkSub120, 0, sizeof(FieldSystemUnkSub120));
    return unkSub120;
}
