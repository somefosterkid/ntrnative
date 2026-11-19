#include "global.h"

#include "touchscreen.h"
#include "unk_02005D10.h"

extern u8 gSystem;

u32 ov56_021E5DA4(u8 *param_1);

u32 ov56_021E5DA4(u8 *param_1) {
    if (((s32 *)(&gSystem))[0x12] != 0) {
        param_1[10] = 0;
    } else if (System_GetTouchHeld() != 0) {
        param_1[10] = 1;
    } else {
        return 0;
    }
    PlaySE(0x5e3);
    return 1;
}
