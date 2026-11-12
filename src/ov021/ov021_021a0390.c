// Black's code at ov021 0x021a0390-0x021a03d0. No name is known: the module and address.

#include "types.h"
#include "field/field_fog.h"

u32 func_ov021_021a0390(FieldFogCtrl *ctrl) {
    if (ctrl->staticLightIndex != 0xfffffff) {
        return 1;
    }
    return 0;
}

u32 func_ov021_021a03a4(FieldFogCtrl *ctrl) {
    if (ctrl->fogData != NULL) {
        return ctrl->fogData[0];
    }
    return 0xfffffff;
}

u32 func_ov021_021a03b8(FieldFogCtrl *ctrl) {
    if (ctrl->fogData != NULL) {
        return ctrl->fogData[1];
    }
    return 0xfffffff;
}

u32 func_ov021_021a03cc(FieldFogCtrl *ctrl) {
    return ctrl->staticLightIndex;
}
