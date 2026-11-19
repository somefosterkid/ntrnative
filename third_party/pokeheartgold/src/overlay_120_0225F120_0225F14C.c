#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "overlay_01_021EFB38.h"
#include "overlay_120.h"

s32 ov120_0225F14C(Ov120Wipe *arg0) {
    s16 temp_r1;
    s16 temp_r3_2;
    u16 temp_r0;

    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (arg0->active == 0) {
        return 1;
    }
    if ((u32) arg0->count < 0x30U) {
        arg0->timer = (s8)(arg0->timer - 1);
        if ((s32) arg0->timer <= 0) {
            arg0->timer = (s8) arg0->interval;
            temp_r0 = ov120_0225F240(&arg0->shuffle);
            temp_r3_2 = (temp_r0 % 8) << 5;
            temp_r1 = (temp_r0 / 8) << 5;
            ov01_021F0960(arg0->window, (s32)temp_r1, (s32)(s16)(temp_r1 + 0x20), (s32)temp_r3_2, (s32)(s16)(temp_r3_2 + 0x20), (u8)(s32) arg0->color);
            arg0->count = (u8)(arg0->count + 1);
        }
    }
    if ((u32) arg0->count >= 0x30U) {
        arg0->active = 0U;
        return 1;
    }
    return 0;
}
