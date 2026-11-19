#include "global.h"

#include "sprite.h"

void sub_02013728(u32);

void ov96_021F8CFC(s32 param_1, s32 param_2, u32 *param_3);

void ov96_021F8CFC(s32 param_1, s32 param_2, u32 *param_3) {
    VecFx32 *pVVar1;
    VecFx32 VStack_20;

    pVVar1 = Sprite_GetMatrixPtr((Sprite *)*param_3);
    VStack_20 = *pVVar1;
    VStack_20.x += param_1 * 0x1000;
    VStack_20.y += param_2 * 0x1000;
    Sprite_SetMatrix((Sprite *)*param_3, &VStack_20);
    pVVar1 = Sprite_GetMatrixPtr((Sprite *)param_3[1]);
    VStack_20 = *pVVar1;
    VStack_20.x += param_1 * 0x1000;
    VStack_20.y += param_2 * 0x1000;
    Sprite_SetMatrix((Sprite *)param_3[1], &VStack_20);
    pVVar1 = Sprite_GetMatrixPtr((Sprite *)param_3[3]);
    VStack_20 = *pVVar1;
    VStack_20.x += param_1 * 0x1000;
    VStack_20.y += param_2 * 0x1000;
    Sprite_SetMatrix((Sprite *)param_3[3], &VStack_20);
    Sprite_SetMatrix((Sprite *)param_3[2], &VStack_20);
    sub_02013728(param_3[4]);
    sub_02013728(param_3[8]);
    return;
}
