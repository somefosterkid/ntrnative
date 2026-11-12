// Black's code at ov021 0x021cd1c8-0x021cd1f0. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_actor.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "struct_decls.h"


FldActMatColorPreset *FldActMatColorPreset_Create(HeapID heapId) {
    FldActMatColorPreset *preset = GFL_HeapAllocate(heapId, sizeof(FldActMatColorPreset), 1, "field_bbd_color.c", 0x5b);

    preset->arcTool = GFL_ArcSysCreateFileHandle(0xaf, heapId);
    return preset;
}
