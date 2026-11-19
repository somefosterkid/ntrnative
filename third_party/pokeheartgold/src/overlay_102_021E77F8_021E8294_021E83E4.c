#include "global.h"

#include "system.h"
#include "touchscreen.h"

s32 ov102_021E83E4(void);

s32 ov102_021E83E4(void) {
    TouchscreenHitbox sp8;
    s32 sp4;
    s32 sp0;
    s32 temp_r0;
    s32 var_r4;
    s32 var_r5;

    if (System_GetTouchNew() == 0) {
        return -1;
    }
    sp4 = 0;
    sp0 = 0x38;
loop_3:
    var_r4 = 0;
    sp8.rect.top = (u8)sp0;
    var_r5 = 0xC;
    sp8.rect.bottom = sp8.rect.top + 0x10;
loop_4:
    sp8.rect.left = var_r5;
    sp8.rect.right = sp8.rect.left + 0x48;
    if (TouchscreenHitbox_PointIsIn(&sp8, (u32)gSystem.touchX, (u32)gSystem.touchY) != 0) {
        return var_r4 + (sp4 * 3);
    }
    var_r4 += 1;
    var_r5 += 0x50;
    if (var_r4 >= 3) {
        sp0 += 0x18;
        temp_r0 = sp4 + 1;
        sp4 = temp_r0;
        if (temp_r0 >= 4) {
            return -1;
        }
        goto loop_3;
    }
    goto loop_4;
}
