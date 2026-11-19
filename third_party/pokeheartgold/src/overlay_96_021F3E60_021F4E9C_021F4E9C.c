#include "global.h"

#include "sprite.h"

Sprite *ov96_021EB5B8(s32 idx);

void ov96_021F4E9C(u8 frame, s32 *posY, s32 idx);

void ov96_021F4E9C(u8 frame, s32 *posY, s32 idx) {
    s32 y, start, diff;
    Sprite *sprite;
    VecFx32 pos;

    if (frame <= 15) {
        start = posY[0];
        diff = posY[1] - start;
        y = start + diff * frame / 15;
    } else if (frame > 15 && frame <= 60) {
        y = posY[1];
    } else {
        start = posY[1];
        diff = posY[2] - start;
        y = start + diff * (u8)(frame - 60) / 10;
    }

    sprite = ov96_021EB5B8(idx);
    pos.x = 128 * FX32_ONE;
    pos.y = y + 512 * FX32_ONE;
    pos.z = 0;
    Sprite_SetMatrix(sprite, &pos);
}
