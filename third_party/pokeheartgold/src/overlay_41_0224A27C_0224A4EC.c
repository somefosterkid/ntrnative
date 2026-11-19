#include "global.h"

#include "touchscreen.h"

extern u8 gSystem;

u32 ov41_0224A4EC(u8 *param_1);

u32 ov41_0224A4EC(u8 *param_1) {
    BOOL BVar1;
    if (((*((s16 *)(param_1 + 0x84))) & 0xFFFF) != 0) {
        return 0;
    }
    if ((*(*((s32 **)(param_1 + 0x80)))) == 1) {
        BVar1 = System_GetTouchHeld();
        if (BVar1 != 0) {
            return 0;
        }
        if (((s32 *)(&gSystem))[0x11] != 0) {
            *(*((u32 **)(param_1 + 0x80))) = 0;
        }
    } else {
        if (((s32 *)(&gSystem))[0x11] != 0) {
            return 0;
        }
        BVar1 = System_GetTouchHeld();
        if (BVar1 != 0) {
            *(*((u32 **)(param_1 + 0x80))) = 1;
        }
    }
    return 0;
}
