#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_96_ov96_icon_work.h"

Ov96IconWork *ov96_021FC188(enum HeapID heapID) {
    Ov96IconWork *work;

    work = Heap_Alloc(heapID, sizeof(Ov96IconWork));
    MIi_CpuClearFast(0, work, sizeof(Ov96IconWork));
    work->heapID = heapID;
    return work;
}
