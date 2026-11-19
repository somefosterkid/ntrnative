#include "global.h"

#include "touchscreen.h"
#include "unk_02005D10.h"

s32 ov43_0222BC78(void *, void *, s32, s32);

s32 ov43_0222C844(void *, s32, s32);

extern TouchscreenHitbox ov43_0222EF60;

s32 ov43_0222BEEC(void *arg0, void *arg1, s32 arg2, s32 *arg3);

s32 ov43_0222BEEC(void *arg0, void *arg1, s32 arg2, s32 *arg3) {
    s32 temp_r0;
    s8 var_r1;

    temp_r0 = TouchscreenHitbox_FindHitboxAtTouchNew(&ov43_0222EF60);
    var_r1 = -1;
    if (temp_r0 == -1) {
        return 0;
    }
    if ((u32)temp_r0 <= 8U) {
        *arg3 = 1;
        *(s16 *)((u8 *)arg0 + 4) = (s16)temp_r0;
        PlaySE(0x5E5U);
        ov43_0222C844(arg0 + 8, arg2, *(s16 *)((u8 *)arg0 + 4));
        return ov43_0222BC78(arg0, arg1, arg2, 5);
    }
    if (temp_r0 != 9) {
        if (temp_r0 == 0xA) {
            var_r1 = 1;
        } else {
            var_r1 = (temp_r0 - 0xB) - *(s16 *)arg0;
        }
    }
    if (var_r1 == 0) {
        return 0;
    }
    *arg3 = 1;
    *(s16 *)((u8 *)arg0 + 2) = (s16) * (s16 *)arg0;
    *(s16 *)arg0 = (s16)(*(s16 *)arg0 + var_r1);
    if ((s32)var_r1 >= 0) {
        *(s16 *)arg0 = (s16)((*(s16 *)arg0) % 4);
        return ov43_0222BC78(arg0, arg1, arg2, 2);
    }
    if ((s32)(*(s16 *)arg0) < 0) {
        *(s16 *)arg0 = (s16)((*(s16 *)arg0) + 4);
    }
    return ov43_0222BC78(arg0, arg1, arg2, 1);
}
