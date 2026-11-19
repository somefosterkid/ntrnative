#include "global.h"

#include "sprite.h"

Sprite *ov96_021E8BAC(s32 param_1);

void ov96_021EAC0C(s32 *param_1, s32 param_2);

void ov96_021EAED4(s32 *param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5);

void ov96_021EAED4(s32 *param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5) {
    Sprite *sprite;
    VecFx32 *pVVar1;
    VecFx32 VStack_1c;
    VecFx32 VStack_28;

    param_1[8] = param_1[8] + param_2;
    param_1[9] = param_1[9] + param_3;
    VStack_1c.x = param_2 << 0xc;
    VStack_1c.y = param_3 << 0xc;
    VStack_1c.z = 0;
    sprite = ov96_021E8BAC(*param_1);
    ov96_021EAC0C(param_1, param_4);
    pVVar1 = Sprite_GetMatrixPtr(sprite);
    VEC_Add(&VStack_1c, pVVar1, &VStack_28);
    Sprite_SetMatrix(sprite, &VStack_28);
    if ((Sprite *)param_1[2] != NULL) {
        pVVar1 = Sprite_GetMatrixPtr((Sprite *)param_1[2]);
        VEC_Add(&VStack_1c, pVVar1, &VStack_28);
        Sprite_SetMatrix((Sprite *)param_1[2], &VStack_28);
    }
    if ((param_5 != 0) && ((Sprite *)param_1[1] != NULL)) {
        pVVar1 = Sprite_GetMatrixPtr((Sprite *)param_1[1]);
        VEC_Add(&VStack_1c, pVVar1, &VStack_28);
        Sprite_SetMatrix((Sprite *)param_1[1], &VStack_28);
    }
    return;
}
