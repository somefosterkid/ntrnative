#include "global.h"

#include "sprite.h"

typedef struct UnkStruct_ov96_021EAF94 {
    void *unk_0;
    Sprite *unk_4;
    Sprite *unk_8;
    u8 unk_C[0x4];
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    u8 unk_1C[0x4];
    s32 unk_20;
    s32 unk_24;
} UnkStruct_ov96_021EAF94;

Sprite *ov96_021E8BAC(void *);

s32 ov96_021E92AC(void *);

void ov96_021EAF94(UnkStruct_ov96_021EAF94 *arg0, s32 arg1, s32 arg2);

void ov96_021EAF94(UnkStruct_ov96_021EAF94 *arg0, s32 arg1, s32 arg2) {
    s32 sp10[3];
    VecFx32 sp4 = { 0, 0, 0 };
    Sprite *sp0;
    Sprite *temp_r0;
    Sprite *temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r2;

    sp0 = ov96_021E8BAC(arg0->unk_0);
    *(VecFx32 *)sp10 = sp4;
    sp10[0] = arg1 << 0xC;
    sp10[1] = arg2 << 0xC;
    temp_r0 = arg0->unk_4;
    if (temp_r0 != NULL) {
        Sprite_SetMatrix(temp_r0, (VecFx32 *)sp10);
    }
    temp_r0_2 = arg0->unk_8;
    if (temp_r0_2 != NULL) {
        Sprite_SetMatrix(temp_r0_2, (VecFx32 *)sp10);
    }
    temp_r0_3 = (arg2 + arg0->unk_10) - (arg0->unk_18 * 2);
    temp_r2 = arg1 - arg0->unk_14;
    arg0->unk_20 = arg1;
    arg0->unk_24 = (s32)(temp_r0_3 + arg0->unk_18);
    sp4.x = temp_r2 << 0xC;
    sp4.y = temp_r0_3 << 0xC;
    if (ov96_021E92AC(arg0->unk_0) != 0) {
        sp4.y += 0x8000;
    }
    Sprite_SetMatrix(sp0, (VecFx32 *)&sp4);
}
