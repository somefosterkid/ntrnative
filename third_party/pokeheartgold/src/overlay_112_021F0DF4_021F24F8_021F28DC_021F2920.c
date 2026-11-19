#include "global.h"
#include "overlay_112_sprite_app.h"

s32 ov112_021F1624(Ov112SpriteApp *, s32, s32);

s32 ov112_021F1F3C(Ov112SpriteRec *, s32);

s32 ov112_021F2204(Ov112SpriteApp *, s32, s32);

void ov112_021F2920(Ov112SpriteApp *arg0);

void ov112_021F2920(Ov112SpriteApp *arg0) {
    ov112_021F2204(arg0, 1, 1);
    ov112_021F1F3C(&arg0->spriteRecs[1], 3);
    ov112_021F1624(arg0, 0x43, 0);
}
