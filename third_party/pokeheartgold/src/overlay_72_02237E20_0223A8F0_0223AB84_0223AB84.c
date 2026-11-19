#include "global.h"

#include "overlay_72_wfc_connect.h"

s32 ov72_0223A738(NintendoWfcConnect *arg0);

void ov72_0223A8F0(NintendoWfcConnect *, s32 arg1);

void ov72_0223AA9C(NintendoWfcConnect *);

void ov72_0223ABB0(NintendoWfcConnect *);

u32 ov72_0223AB84(NintendoWfcConnect *param_1);

u32 ov72_0223AB84(NintendoWfcConnect *param_1) {
    s32 iVar1;

    iVar1 = ov72_0223A738(param_1);
    if (iVar1 == -1) {
        ov72_0223AA9C(param_1);
    } else {
        ov72_0223A8F0(param_1, iVar1);
    }
    ov72_0223ABB0(param_1);
    return 0;
}
