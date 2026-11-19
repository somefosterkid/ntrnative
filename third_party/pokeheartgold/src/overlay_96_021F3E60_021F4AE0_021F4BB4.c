#include "global.h"

#include "bg_window.h"
#include "sprite.h"

typedef struct UnkStruct_ov96_021F4BB4 {
    u8 unk_0[0x8];
    BgConfig *unk_8;
    u8 unk_C[0xE];
    u16 unk_1A;
    u8 unk_1C[0x4C];
    u8 unk_68;
    u8 unk_69[0x5B];
    void *unk_C4;
} UnkStruct_ov96_021F4BB4;

void ov96_021F4724(void *arg0, s32);

s32 ov96_021EB52C(void *, s32, s32);

Sprite *ov96_021EB5B8(void *);

void ov96_021F4BB4(UnkStruct_ov96_021F4BB4 *arg0, s32 arg1);

void ov96_021F4BB4(UnkStruct_ov96_021F4BB4 *arg0, s32 arg1) {
    s32 sp4[7];
    UnkStruct_ov96_021F4BB4 *temp_r4;
    u16 temp_r4_2;
    s32 temp_r1;
    u16 temp_r0;
    u16 temp_r0_2;
    s32 temp_r0_3;

    temp_r4 = (void *)&arg0->unk_68;
    temp_r4->unk_1A = (u16)(temp_r4->unk_1A + 1);
    temp_r0 = temp_r4->unk_1A;
    if ((u32)temp_r0 > 0x50U) {
        ov96_021F4724(temp_r4, arg1);
        return;
    }
    if (temp_r0 == 0x29) {
        ov96_021EB52C(temp_r4->unk_C4, 1, 1);
    } else if (temp_r0 == 0x3D) {
        ov96_021EB52C(temp_r4->unk_C4, 1, 0);
    }
    temp_r0_2 = temp_r4->unk_1A;
    if ((u32)temp_r0_2 <= 0x28U) {
        temp_r0_3 = (s32)(4096.0f * ((f32)temp_r0_2 / 40.0f));
        (sp4 + 3)[0] = temp_r0_3;
        sp4[4] = 0;
        sp4[5] = 0;
        sp4[6] = temp_r0_3;
        OS_WaitVBlankIntr();
        SetBgAffine(arg0->unk_8, 7U, (MtxFx22 *)(sp4 + 3), 0x80, 0x60);
        return;
    }
    if (((u32)temp_r0_2 > 0x28U) && ((u32)temp_r0_2 <= 0x3CU)) {
        sp4[2] = 0;
        temp_r1 = 0x90 * (temp_r0_2 - 0x28);
        sp4[0] = ((temp_r1 / 20) + 0x38) << 0xC;
        sp4[1] = 0x290000;
        Sprite_SetMatrix(ov96_021EB5B8(temp_r4->unk_C4), (VecFx32 *)&sp4[0]);
        return;
    }
    ov96_021EB52C(temp_r4->unk_C4, 1, 0);
    temp_r4_2 = (u16)((s32)((temp_r4->unk_1A - 0x3C) * 0x10) / 20);
    G2S_SetBlendAlpha(8, 0x24, 0x10 - temp_r4_2, (s32)temp_r4_2);
}
