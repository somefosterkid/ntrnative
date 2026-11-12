// Black's code at ov021 0x021c63ac-0x021c6400. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_g3dobj.h"
#include "gfl/heap.h"

FieldG3DObjSystem *func_ov021_021c63ac(HeapID heapId, s32 resGroupCount, s32 objCount) {
    FieldG3DObjSystem *sys = GFL_HeapAllocate(heapId, sizeof(FieldG3DObjSystem), 1, "field_g3dobj.c", 0x72);

    sys->resGroups = GFL_HeapAllocate(heapId, resGroupCount * sizeof(FieldG3DObjResGroup), 1, "field_g3dobj.c", 0x75);
    sys->objs = GFL_HeapAllocate(heapId, objCount * sizeof(FieldG3DObj), 1, "field_g3dobj.c", 0x78);
    sys->unk_00 = heapId;
    sys->resGroupCount = resGroupCount;
    sys->objCount = objCount;
    sys->unk_02 = 1;
    return sys;
}
