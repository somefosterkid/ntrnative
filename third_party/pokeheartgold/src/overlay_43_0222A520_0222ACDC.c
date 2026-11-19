#include "global.h"

#include "sprite.h"
#include "overlay_43_palpad.h"

void ov43_0222ACDC(PalPadGfx *gfx);

void ov43_0222ACDC(PalPadGfx *gfx) {
    s32 i;

    Sprite_Delete(gfx->cursor);
    for (i = 0; i < 2; i++) {
        Sprite_Delete(gfx->arrows[i]);
    }
}

void ov43_0222AD00(PalPadGfx *gfx, s32 flag);

void ov43_0222AD00(PalPadGfx *gfx, s32 flag) {
    s32 i;

    for (i = 0; i < 2; i++) {
        Sprite_SetDrawFlag(gfx->arrows[i], flag);
    }
}

void ov43_0222AD20(PalPadGfx *gfx, s32 flag);

void ov43_0222AD20(PalPadGfx *gfx, s32 flag) {
    s32 i;

    for (i = 0; i < 2; i++) {
        Sprite_SetAnimActiveFlag(gfx->arrows[i], flag);
    }
}
