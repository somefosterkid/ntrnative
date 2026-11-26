#include "global.h"
#include "overlay_96_sprite_res_man.h"
#include "sprite.h"

u32 ov96_021EB0CC(Ov96SpriteSetElem *elem, fx32 param_2, fx32 param_3, s32 param_4, s32 param_5);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_021E9204(void *param_1, f32 x, f32 y);
#else
void ov96_021E9204(void *param_1, u32 param_2, u32 param_3);
#endif
void ov96_021EB10C(Ov96SpriteSetElem *elem, u32 param_2, u32 param_3);
s32 ov96_021EB120(Ov96SpriteSetElem *elem);

u32 ov96_021EB0CC(Ov96SpriteSetElem *elem, fx32 param_2, fx32 param_3, s32 param_4, s32 param_5) {
    fx32 fVar1;
    VecFx32 VStack_18;
    VecFx32 VStack_24;
    VecFx32 VStack_30;

    VStack_30.x = param_4 << 0xc;
    VStack_30.y = param_5 << 0xc;
    VStack_30.z = 0;
    VStack_24.z = 0;
    VStack_24.x = param_2;
    VStack_24.y = param_3;
    VEC_Subtract(&VStack_30, &VStack_24, &VStack_18);
    fVar1 = VEC_Mag(&VStack_18);
    if (fVar1 < elem->unk_1C[0] * 0x1000) {
        return 1;
    }
    return 0;
}

void ov96_021EB10C(Ov96SpriteSetElem *elem, u32 param_2, u32 param_3) {
    Ov96SpriteResEntry *entry;

    entry = ov96_021EAA20(elem);
    ov96_021E9204(entry, param_2, param_3);
    return;
}

s32 ov96_021EB120(Ov96SpriteSetElem *elem) {
    if (Sprite_GetDrawFlag(ov96_021E8BAC(elem->entry)) != 0) {
        return 1;
    }
    return 0;
}
