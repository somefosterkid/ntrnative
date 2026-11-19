#include "global.h"

#include "error_handling.h"

s32 ov01_021F0D20(u32 *param_1);

s32 ov01_021F0D20(u32 *param_1) {
    u32 uVar1;
    s32 iVar2;
    s32 unaff_r4;

    uVar1 = *param_1;
    iVar2 = 1;
    if ((uVar1 & 1) == 0) {
        if (((uVar1 & 0x1720) != 0) || (uVar1 == 0)) {
            iVar2 = 0;
        } else {
            GF_AssertFail();
            iVar2 = 0;
        }
    }
    switch (param_1[0x54]) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 8:
    case 9:
    case 10:
    case 0xb:
    case 0x18:
        unaff_r4 = 0;
        break;
    case 7:
        unaff_r4 = 2;
        break;
    case 5:
        unaff_r4 = 4;
        break;
    }
    if (param_1[0x57] - 3 <= 1) {
        unaff_r4 = unaff_r4 + 1;
    }
    return unaff_r4 + iVar2 * 6;
}
