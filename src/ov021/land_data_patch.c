// Black's code at ov021 0x021c939c-0x021c93d0. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_map.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"

// A land data patch: a block of heapId that holds a file of archive 0x9c, read into the heap
LandDataPatch *func_ov021_021c939c(u32 fileId, HeapID heapId) {
    LandDataPatch *patch = GFL_HeapAllocate(heapId, sizeof(LandDataPatch), TRUE, "land_data_patch.c", 0x4f);

    patch->data = GFL_ArcSysReadHeapNewLZ(0x9c, fileId, FALSE, heapId);
    return patch;
}
