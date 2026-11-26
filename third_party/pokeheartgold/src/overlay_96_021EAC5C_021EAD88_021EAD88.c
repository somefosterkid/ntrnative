#include "global.h"

#include "sprite.h"
#include "field_types_def.h"

Sprite *ov96_021E8BAC(s32 param_1);

u32 ov96_021E92AC(s32 param_1);

void ov96_021EAD88(s32 *param_1, s32 param_2, s32 param_3, s32 param_4);

void ov96_021EAD88(s32 *param_1, s32 param_2, s32 param_3, s32 param_4) {
    Sprite *sprite;
    s32 iVar1;
    VecFx32 VStack_20;
    VecFx32 VStack_2c = { 0, 0, 0 };

    sprite = ov96_021E8BAC(*param_1);
    VStack_2c.x = param_2 * 0x1000;
    VStack_2c.y = param_3 * 0x1000;
    param_1[8] = param_2 + param_1[5];
    param_1[9] = param_3 + param_1[6];
    if (param_1[2] != 0) {
        VStack_20 = VStack_2c;
        VStack_20.x += param_1[5] * 0x1000;
        VStack_20.y += (param_1[6] * 2 - param_1[3]) * 0x1000;
        Sprite_SetMatrix((Sprite *)param_1[2], &VStack_20);
    }
    if ((param_4 != 0) && (param_1[1] != 0)) {
        VStack_20 = VStack_2c;
        VStack_20.x += param_1[5] * 0x1000;
        VStack_20.y += (param_1[6] * 2 - param_1[4]) * 0x1000;
        Sprite_SetMatrix((Sprite *)param_1[1], &VStack_20);
    }
    iVar1 = ov96_021E92AC(*param_1);
    if (iVar1 != 0) {
        VStack_2c.y = VStack_2c.y + 0x8000;
    }
    Sprite_SetMatrix(sprite, &VStack_2c);
    return;
}
