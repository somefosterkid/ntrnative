#include "global.h"

#include "sprite.h"

void ov96_021FBE4C(s32 param_1, void *param_2, void *param_3);

void ov96_021FBE4C(s32 param_1, void *param_2, void *param_3) {
    *(void **)(param_1 + 4) = param_2;
    *(void **)(param_1 + 8) = param_3;
    return;
}

void ov96_021EB52C(s16 *param_1, s32 param_2, s32 param_3);

struct Sprite *ov96_021EB5B8(s32 param_1);

void ov96_021FBE54(u32 *param_1, s32 param_2);

void ov96_021FBE54(u32 *param_1, s32 param_2) {
    Sprite *sprite;

    if (param_2 != 0) {
        *param_1 = 1;
        param_1[3] = 1;
        ov96_021EB52C((s16 *)param_1[1], 1, 1);
        ov96_021EB52C((s16 *)param_1[2], 1, 0);
        sprite = ov96_021EB5B8(param_1[2]);
        Sprite_SetAnimActiveFlag(sprite, 0);
        return;
    }
    *param_1 = 0;
    param_1[3] = 0;
    ov96_021EB52C((s16 *)param_1[1], 0, 0);
    ov96_021EB52C((s16 *)param_1[2], 0, 0);
    return;
}

u32 ov96_021FBE9C(u32 *param_1);

u32 ov96_021FBE9C(u32 *param_1) {
    return *param_1;
}

u32 ov96_021FBEA0(u8 *param_1);

u32 ov96_021FBEA0(u8 *param_1) {
    return *(u32 *)(param_1 + 0xc);
}

void ov96_021FBEA4(u8 *param_1);

void ov96_021FBEA4(u8 *param_1) {
    Sprite *sprite;

    *(u32 *)(param_1 + 0xc) = 0;
    sprite = ov96_021EB5B8(*(s32 *)(param_1 + 8));
    ov96_021EB52C(*(s16 **)(param_1 + 4), 1, 0);
    ov96_021EB52C(*(s16 **)(param_1 + 8), 1, 1);
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimCtrlSeq(sprite, 1);
    return;
}

void ov96_021FBEDC(u8 *param_1, VecFx32 *param_2);

void ov96_021FBEDC(u8 *param_1, VecFx32 *param_2) {
    Sprite *pSVar1;

    pSVar1 = ov96_021EB5B8(*(s32 *)(param_1 + 4));
    Sprite_SetMatrix(pSVar1, param_2);
    pSVar1 = ov96_021EB5B8(*(s32 *)(param_1 + 8));
    Sprite_SetMatrix(pSVar1, param_2);
    return;
}
