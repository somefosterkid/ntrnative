#include "global.h"
#include "overlay_112_sprite_app.h"

s32 ov112_021F1624(Ov112SpriteApp *, s32, s32);

s32 ov112_021F2204(Ov112SpriteApp *, s32, s32);

s32 ov112_021F2348(Ov112SpriteApp *);

void ov112_021F27B8(Ov112SpriteApp *arg0);

s32 ov112_021F22D0(Ov112SpriteApp *, s32, s32, s32);
void ov112_021F27E0(Ov112SpriteApp *arg0);

void ov112_021F27B8(Ov112SpriteApp *arg0) {
    ov112_021F2204(arg0, 0, 1);
    ov112_021F2204(arg0, 1, 0);
    ov112_021F2348(arg0);
    ov112_021F1624(arg0, 0x2C, 0);
}

void ov112_021F27E0(Ov112SpriteApp *arg0) {
    ov112_021F2204(arg0, 0, 1);
    ov112_021F2204(arg0, 1, 0);
    ov112_021F22D0(arg0, 0, 9, 0x1A);
    ov112_021F1624(arg0, 0x2D, 0);
}
