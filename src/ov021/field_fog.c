// Black's code at ov021 0x021a0104-0x021a0138. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_fog.h"
#include "gfl/heap.h"

void FieldFog_SetDefaultTable(FieldFog *fog);

FieldFog *func_ov021_021a0104(HeapID heapId) {
    FieldFog *fog = GFL_HeapAllocate(heapId, sizeof(FieldFog), 1, "field_fog.c", 0x89);

    fog->alphaMode = 0;
    fog->alpha = 0x1f;
    fog->offset = 0x7fff;
    FieldFog_SetDefaultTable(fog);
    return fog;
}
