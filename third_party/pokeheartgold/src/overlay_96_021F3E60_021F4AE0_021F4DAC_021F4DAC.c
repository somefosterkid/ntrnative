#include "global.h"

#include "sprite.h"

Sprite *ov96_021EB5B8(s32);

void ov96_021F4DAC(u32 arg0, VecFx32 *arg1, s32 arg2);

void ov96_021F4DAC(u32 arg0, VecFx32 *arg1, s32 arg2) {
    VecFx32 sp4;
    s32 x;
    s32 y;
    s32 x0;
    s32 y0;
    s32 dx;
    s32 dy;
    u8 t;
    Sprite *sprite;

    if (arg0 <= 5) {
        x0 = arg1[0].x;
        y0 = arg1[0].y;
        dx = arg1[1].x - x0;
        x = x0 + (s32)(dx * arg0) / 5;
        dy = arg1[1].y - y0;
        y = y0 + (s32)(dy * arg0) / 5;
    } else if (arg0 > 5 && arg0 <= 45) {
        t = arg0 - 5;
        x0 = arg1[1].x;
        y0 = arg1[1].y;
        dx = arg1[2].x - x0;
        x = x0 + (dx * t) / 40;
        dy = arg1[2].y - y0;
        y = y0 + (dy * t) / 40;
    } else {
        t = arg0 - 45;
        x0 = arg1[2].x;
        y0 = arg1[2].y;
        dx = arg1[3].x - x0;
        x = x0 + (dx * t) / 5;
        dy = arg1[3].y - y0;
        y = y0 + (dy * t) / 5;
    }
    sprite = ov96_021EB5B8(arg2);
    sp4.y = y + 0x200000;
    sp4.z = 0;
    sp4.x = x;
    Sprite_SetMatrix(sprite, &sp4);
}
