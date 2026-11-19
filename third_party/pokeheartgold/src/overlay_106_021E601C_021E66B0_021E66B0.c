#include "global.h"

#include "follow_mon.h"

s32 ov106_021E66B0(s32 param_1, s32 param_2, s32 param_3);

s32 ov106_021E66B0(s32 param_1, s32 param_2, s32 param_3) {
    BOOL BVar2;
    s32 iVar1;
    s32 iVar3;
    if ((param_1 <= 0) || (0x1ed < param_1)) {
        iVar3 = 1;
    } else {
        iVar1 = SpeciesToOverworldModelIndexOffset(param_1);
        iVar3 = iVar1 + 0x129;
        iVar1 = OverworldModelLookupHasFemaleForm(param_1);
        BVar2 = iVar1;
        iVar1 = 0;
        if (BVar2 != iVar1) {
            if (param_3 == 1) {
                iVar3 = iVar3 + 1;
            }
        } else {
            iVar1 = OverworldModelLookupFormCount(param_1);
            if (param_2 > iVar1) {
                param_2 = 0;
            }
            iVar3 = iVar3 + param_2;
        }
    }
    return iVar3;
}
