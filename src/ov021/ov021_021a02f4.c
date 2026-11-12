// Black's code at ov021 0x021a02f4-0x021a0380. No name is known: the module and address.

#include "types.h"
#include "field/field_fog.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "struct_decls.h"

void func_ov021_021a02f4(FieldFogCtrl *ctrl) {
    GFL_ArcToolFree(ctrl->arc);
    FieldFogCtrl_Reset(ctrl);
    GFL_HeapFree(ctrl);
}

void func_ov021_021a030c(FieldFogCtrl *ctrl, HeapID heapId) {
    if (ctrl->loadRequested == 1) {
        ctrl->loadDelay = ctrl->loadDelay - 1;
        if (ctrl->loadDelay <= 0) {
            ctrl->fogData = GFL_ArcToolReadHeapNewLZ(ctrl->arc, ctrl->fileId, FALSE, heapId);
            ctrl->loadRequested = 0;
        }
    }
}


void func_ov021_021a0338(FieldFogCtrl *ctrl, s32 fileId, u32 staticLightIndex, u8 delay) {
    if (fileId != 0xfffffff) {
        ctrl->loadRequested = 1;
        ctrl->fileId = (s16)fileId;
        ctrl->loadDelay = delay;
    }
    ctrl->staticLightIndex = staticLightIndex;
}

u8 func_ov021_021a0354(FieldFogCtrl *ctrl) {
    return ctrl->loadRequested;
}

void FieldFogCtrl_Reset(FieldFogCtrl *ctrl) {
    if (ctrl->fogData != NULL) {
        GFL_HeapFree(ctrl->fogData);
        ctrl->fogData = NULL;
    }
    ctrl->staticLightIndex = 0xfffffff;
    if (ctrl->loadRequested == 1) {
        ctrl->loadRequested = 0;
    }
}
