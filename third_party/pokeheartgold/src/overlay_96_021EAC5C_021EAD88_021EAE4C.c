#include "global.h"

#include "sprite.h"

Sprite *ov96_021E8BAC(s32 param_1);

u32 ov96_021E92AC(s32 param_1);

void ov96_021EAE4C(s32 *param_1, s32 param_2, s32 param_3);

void ov96_021EAE4C(s32 *param_1, s32 param_2, s32 param_3) {
    Sprite *sprite;
    s32 iVar1;
    VecFx32 VStack_20 = { 0, 0, 0 };
    sprite = ov96_021E8BAC(*param_1);
    VStack_20.x = param_2 << 0xc;
    VStack_20.y = param_3 << 0xc;
    param_1[8] = param_2 + param_1[5];
    param_1[9] = param_3 + param_1[6];
    iVar1 = ov96_021E92AC(*param_1);
    if (iVar1 != 0) {
        VStack_20.y = VStack_20.y + 0x8000;
    }
    Sprite_SetMatrix(sprite, &VStack_20);
    return;
}
