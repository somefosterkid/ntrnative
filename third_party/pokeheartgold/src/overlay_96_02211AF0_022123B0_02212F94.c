#include "global.h"

#include "overlay_96_app_02210C98.h"

extern s32 ov96_0221D268;

extern s32 ov96_0221D280;

void ov96_02212F94(Ov96App_02210C98_Mover *arg0, Ov96App_02210C98_Mover *arg1, u8 arg2);

void ov96_02212F94(Ov96App_02210C98_Mover *arg0, Ov96App_02210C98_Mover *arg1, u8 arg2) {
    VecFx32 sp1C = { 0, 0, 0 };
    VecFx32 sp10;
    VecFx32 sp4;
    s32 sp0 = arg2;
    VecFx32 *temp_r0;
    u8 temp_r0_2;
    if (((u32)arg0->unk_40) <= 3U) {
        if (((u32)arg1->unk_40) > 3U) {
            arg0->unk_39 = (u8)(arg1->unk_39);
            arg0->unk_3C = (u8)(arg1->unk_3C);
            arg0->unk_3D = (u8)(arg1->unk_3D);
            arg0->unk_3E = (u8)(arg1->unk_3E);
            arg0->unk_40 = (u8)(arg1->unk_40);
            arg0->unk_41 = (u8)(arg1->unk_41);
        } else {
            arg0->unk_39 = 0U;
            arg0->unk_3C = 0U;
            arg0->unk_3D = 0x1EU;
            arg0->unk_3E = 1U;
            arg0->unk_40 = 3U;
            arg0->unk_41 = 0U;
        }
    } else {
        arg0->unk_39 = 0U;
        arg0->unk_3C = 0U;
        arg0->unk_3D = 0x1EU;
        arg0->unk_3E = 1U;
        arg0->unk_40 = 3U;
        arg0->unk_41 = 0U;
    }
    VEC_Subtract(&arg0->unk_8, &arg1->unk_8, &arg0->unk_20);
    if (VEC_Mag(&arg0->unk_20) == 0) {
        sp10 = *((VecFx32 *)(&ov96_0221D268));
        sp4 = *((VecFx32 *)(&ov96_0221D280));
        if (sp0 == 0) {
            arg0->unk_20 = sp10;
        } else {
            arg0->unk_20 = sp4;
        }
    }
    temp_r0 = &arg0->unk_20;
    VEC_Normalize(temp_r0, temp_r0);
    VEC_MultAdd(arg0->unk_40 << 0xC, &arg0->unk_20, &sp1C, &arg0->unk_14);
    temp_r0_2 = arg1->unk_3B;
    if (((u32)temp_r0_2) < 0xCU) {
        arg0->unk_3B = temp_r0_2;
    }
}
