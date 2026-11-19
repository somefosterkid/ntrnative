#include "global.h"

#include "sprite.h"

typedef struct UnkStruct_ov96_021EAEC8 {
    u8 unk_0[0x20];
    s32 unk_20;
    s32 unk_24;
} UnkStruct_ov96_021EAEC8;

Sprite *ov96_021E8BAC(s32 param_1);

u32 ov96_021EAA20(u32 *param_1);

void ov96_021EAE9C(u32 *param_1, s32 *param_2, s32 *param_3);

void ov96_021EAE9C(u32 *param_1, s32 *param_2, s32 *param_3) {
    s32 iVar1;
    Sprite *sprite;
    VecFx32 *pVVar2;

    iVar1 = ov96_021EAA20(param_1);
    sprite = ov96_021E8BAC(iVar1);
    pVVar2 = Sprite_GetMatrixPtr(sprite);
    *param_2 = (s32)(pVVar2->x + ((u32)(pVVar2->x >> 0xb) >> 0x14)) >> 0xc;
    *param_3 = (s32)(pVVar2->y + ((u32)(pVVar2->y >> 0xb) >> 0x14)) >> 0xc;
    return;
}

void ov96_021EAEC8(UnkStruct_ov96_021EAEC8 *arg0, s32 *arg1, s32 *arg2);

void ov96_021EAEC8(UnkStruct_ov96_021EAEC8 *arg0, s32 *arg1, s32 *arg2) {
    *arg1 = arg0->unk_20;
    *arg2 = arg0->unk_24;
}
