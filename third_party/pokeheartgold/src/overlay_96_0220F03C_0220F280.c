#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

#include "error_handling.h"
#include "filesystem.h"
#include "pokeathlon/pokeathlon.h"

UnkStruct_ov96_0220E3F0_Cell *ov96_0220F378(PokeathlonCourseData *, s32, u32);

void ov96_0220F280(PokeathlonCourseData *arg0);

void ov96_0220F280(PokeathlonCourseData *arg0) {
    s32 sp8[20];
    s32 sp4;
    s32 sp0;
    f32 var_r0;
    f32 var_r0_2;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r0_3;
    u32 var_r6;
    PokeathlonMonKinds *temp_r4;
    UnkStruct_ov96_0220E3F0_Cell *temp_r5;

    ReadWholeNarcMemberByIdPair(&sp8[0], NARC_a_1_7_0, 7);
    sp0 = 0;
    do {
        var_r6 = 0;
        sp4 = (s32)(u8)sp0;
loop_2:
        temp_r4 = ov96_021E60D8(arg0, sp0, var_r6);
        temp_r5 = ov96_0220F378(arg0, sp4, (u8)var_r6);
        if ((u32)temp_r4->statIdx[2] >= 5U) {
            GF_AssertFail();
        }
        if ((u32)temp_r4->statIdx[0] >= 5U) {
            GF_AssertFail();
        }
        if ((u32)temp_r4->statIdx[1] >= 5U) {
            GF_AssertFail();
        }
        temp_r5->unk_2 = temp_r4->statIdx[2];
        temp_r5->unk_0 = (s8)(&sp8[0])[temp_r4->statIdx[2]];
        temp_r5->unk_1 = (s8) * (s32 *)((u8 *)(&(&sp8[0])[temp_r4->statIdx[2]]) + 0x14);
        temp_r0_2 = *(s32 *)((u8 *)(&(&sp8[0])[temp_r4->statIdx[0]]) + 0x28);
        if (temp_r0_2 > 0) {
            var_r0 = 0.5f + (f32)(temp_r0_2 << 0xC);
        } else {
            var_r0 = (f32)(temp_r0_2 << 0xC) - 0.5f;
        }
        temp_r5->unk_4 = (s32)((s32)var_r0 / 10);
        temp_r0_3 = *(s32 *)((u8 *)(&(&sp8[0])[temp_r4->statIdx[1]]) + 0x3C);
        if (temp_r0_3 > 0) {
            var_r0_2 = 0.5f + (f32)(temp_r0_3 << 0xC);
        } else {
            var_r0_2 = (f32)(temp_r0_3 << 0xC) - 0.5f;
        }
        temp_r5->unk_8 = (s32)((s32)var_r0_2 / 10);
        var_r6 += 1;
        if ((s32)var_r6 < 3) {
            goto loop_2;
        }
        temp_r0 = sp0 + 1;
        sp0 = temp_r0;
    } while (temp_r0 < 4);
}
