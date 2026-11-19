#include "global.h"

#include "error_handling.h"

u32 ov96_02217E08(u8 *param_1);

u32 ov96_02217E08(u8 *param_1) {
    s32 iVar1;
    s32 iVar2;
    u32 uVar2;

    iVar2 = *(s32 *)(param_1 + 0x14);
    iVar1 = *(s32 *)(param_1 + 0x18);
    uVar2 = 0;
    if (!(((u32)(*(s32 *)(param_1 + 0x60) << 4)) >> 31)) {
        return uVar2;
    }
    {
        switch (iVar2) {
        case 0xb:
            uVar2 = 0;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 0xc:
            if ((iVar1 == 0xb) || (iVar1 == 6)) {
                uVar2 = 3;
            } else if (iVar2 == 6) {
                uVar2 = 2;
            } else {
                uVar2 = 1;
            }
            break;
        case 1:
        case 2:
        case 9:
            if ((iVar1 == 0xb) || (iVar1 == 6)) {
                uVar2 = 3;
            }
            break;
        case 0:
        case 3:
        case 4:
        case 10:
            GF_AssertFail();
            break;
        default:
            GF_AssertFail();
        }
        return uVar2;
    }
}
