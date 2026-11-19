#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "unk_02005D10.h"

void ov59_02238834(Ov59ApricornApp *, u32, u32);

void ov59_02238A04(Ov59ApricornApp *, s32);

s32 ov59_0223A564(s32 param_1, s32 param_2, s32 param_3, s32);

u32 ov59_0223A48C(Ov59ApricornApp *param_1, s32 param_2, s32 arg2, s32 arg3);

u32 ov59_0223A48C(Ov59ApricornApp *param_1, s32 param_2, s32 arg2, s32 arg3) {
    s32 iVar1;

    if (param_2 == 0) {
        PlaySE(0x5dc);
        ov59_02238834(param_1, 0, 1);
        return 0;
    }
    if (param_2 == 1) {
        PlaySE(0x5dc);
        ov59_02238834(param_1, 1, 1);
        return 1;
    }
    switch (param_2) {
    case 2:
        iVar1 = ov59_0223A564((u32) param_1->quantity, (u32) param_1->maxQuantity, 1, arg3);
        param_1->quantity = (s8)iVar1;
        break;
    case 4:
        iVar1 = ov59_0223A564((u32) param_1->quantity, (u32) param_1->maxQuantity, 10, arg3);
        param_1->quantity = (s8)iVar1;
        break;
    case 3:
        iVar1 = ov59_0223A564((u32) param_1->quantity, (u32) param_1->maxQuantity, -1, arg3);
        param_1->quantity = (s8)iVar1;
        break;
    case 5:
        iVar1 = ov59_0223A564((u32) param_1->quantity, (u32) param_1->maxQuantity, -10, arg3);
        param_1->quantity = (s8)iVar1;
        break;
    default:
        return 6;
    }
    PlaySE(0x5dc);
    ov59_02238A04(param_1, param_1->quantity);
    return 6;
}
