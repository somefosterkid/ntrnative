#include "global.h"
#include "overlay_59_juice_feed_app.h"

#include "unk_02005D10.h"

s32 ov59_0223B2B4(Ov59JuiceFeedApp *arg0, u32 arg1);

s32 ov59_0223B2B4(Ov59JuiceFeedApp *arg0, u32 arg1) {
    u8 temp_r1;

    switch (arg1) {
    case 0:
        ov59_0223BAE8(arg0);
        PlaySE(0x5DCU);
        ov59_0223AEB0(arg0, 0);
        return 4;
    case 1:
        PlaySE(0x5DCU);
        ov59_0223AEB0(arg0, 1);
        return 1;
    case 2:
        temp_r1 = arg0->monCount;
        arg0->selected = (u8)((s32)((arg0->selected - 1) + temp_r1) % (s32)temp_r1);
        PlaySE(0x5DCU);
        ov59_0223BC88(arg0, arg0->selected);
        return 2;
    case 3:
        arg0->selected = (u8)((s32)(arg0->selected + 1) % (s32) arg0->monCount);
        PlaySE(0x5DCU);
        ov59_0223BC88(arg0, arg0->selected);
        return 2;
    default:
        return 2;
    }
}
