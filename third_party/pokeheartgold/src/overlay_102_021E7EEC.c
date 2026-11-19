#include "global.h"

#include "touchscreen.h"

extern u8 gSystem;

u32 ov102_021E7EEC(void *param_1, s32 param_2, s32 param_3);

u32 ov102_021E7F30(void *param_1, s32 param_2, s32 param_3);

u32 ov102_021E7EEC(void *param_1, s32 param_2, s32 param_3) {
    BOOL BVar1;
    u32 ret;
    BVar1 = System_GetTouchHeld();
    if (BVar1 == 0) {
        ret = 0;
    } else if ((param_2 < ((s32)(((u16 *)(&gSystem))[0x30] - 10))) || (param_2 > ((s32)(((u16 *)(&gSystem))[0x30] + 10)))) {
        ret = 0;
    } else if ((param_3 < ((s32)(((u16 *)(&gSystem))[0x31] - 0xc))) || (param_3 > ((s32)(((u16 *)(&gSystem))[0x31] + 0xc)))) {
        ret = 0;
    } else {
        ret = 1;
    }
    return ret;
}

u32 ov102_021E7F30(void *param_1, s32 param_2, s32 param_3) {
    BOOL BVar1;
    u32 ret;
    BVar1 = System_GetTouchHeld();
    if (BVar1 == 0) {
        ret = 0;
    } else if (((0x11 + 1) > param_2) || (param_2 > (0xed - 1))) {
        ret = 0;
    } else if ((param_3 < ((s32)(((u16 *)(&gSystem))[0x31] - 0x18))) || (param_3 > ((s32)(((u16 *)(&gSystem))[0x31] + 0x18)))) {
        ret = 0;
    } else {
        ret = 1;
    }
    return ret;
}
