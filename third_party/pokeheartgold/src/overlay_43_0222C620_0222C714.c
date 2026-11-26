#include "global.h"

#include "sprite.h"
#include "unk_02005D10.h"
#include "overlay_43_palpad.h"
#include "overlay_43_palpad_fns.h"

BOOL ov43_0222C714(PalPadRoster *arg0, PalPadGfx *gfx) {
    u16 v0;

    if (Sprite_IsAnimated(arg0->deleteEffect) == 0) {
        return 1;
    }
    v0 = Sprite_GetAnimationFrame(arg0->deleteEffect);
    if ((u32)(v0 - arg0->lastFrame) >= 2U) {
        arg0->lastFrame = v0;
        PlaySE(0x628U);
    }
    return 0;
}

s32 ov43_0222AD74(void *, s32, s32);

void ov43_0222C750(struct PalPadRoster *arg0, void *arg1);

void ov43_0222C750(PalPadRoster *arg0, void *arg1) {
    reg_GX_DISPCNT &= 0xFFFF1FFF;
    Sprite_SetDrawFlag(arg0->deleteEffect, 0);
    Sprite_SetAnimActiveFlag(arg0->deleteEffect, 0);
    ov43_0222AD74(arg1, 1, 0xFF);
}
