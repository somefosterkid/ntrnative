// Black's code at ov021 0x021a02c4-0x021a02f4. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_fog.h"
#include "gfl/arc.h"
#include "gfl/heap.h"

FieldFogCtrl *func_ov021_021a02c4(HeapID heapId) {
    FieldFogCtrl *ctrl;

    ctrl = GFL_HeapAllocate(heapId, sizeof(FieldFogCtrl), 1, "field_zonefog.c", 0x51);
    ctrl->staticLightIndex = 0xfffffff;
    ctrl->arc = GFL_ArcSysCreateFileHandle(0x51, heapId);
    return ctrl;
}
