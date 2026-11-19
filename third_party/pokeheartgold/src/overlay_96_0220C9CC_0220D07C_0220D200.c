#include "global.h"

#include "error_handling.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "pokeathlon/pokeathlon.h"

void ov96_0220D200(u32 *arg0, s32 arg1, PokeathlonCourseData *arg2);

void ov96_0220D200(u32 *arg0, s32 arg1, PokeathlonCourseData *arg2) {
    s32 sp8[15];
    PokeathlonCourseData *sp4;
    s32 sp0;
    u32 *var_r5;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r1_3;
    s32 var_r6;
    PokeathlonMonKinds *temp_r0;
    var_r5 = arg0;
    sp0 = arg1;
    sp4 = arg2;
    ReadWholeNarcMemberByIdPair(&sp8[0], NARC_a_1_7_0, 5);
    var_r6 = 0;
    do {
        temp_r0 = ov96_021E60D8(sp4, sp0, var_r6);
        if (((u32)(temp_r0->statIdx[0])) >= 5U) {
            GF_AssertFail();
        }
        if (((u32)(temp_r0->statIdx[3])) >= 5U) {
            GF_AssertFail();
        }
        if (((u32)(temp_r0->statIdx[4])) >= 5U) {
            GF_AssertFail();
        }
        var_r6 += 1;
        temp_r1 = (0x1FF & (&sp8[0])[temp_r0->statIdx[0]]) | (0xFFFFFE00 & (*var_r5));
        *var_r5 = temp_r1;
        temp_r1_2 = (((u32)((*((s32 *)(((u8 *)(&(&sp8[0])[temp_r0->statIdx[3]])) + 0x14))) << 0x17)) >> 0xE) | (0xFFFC01FF & temp_r1);
        *var_r5 = temp_r1_2;
        temp_r1_3 = (*var_r5 = ((*((s32 *)(((u8 *)(&(&sp8[0])[temp_r0->statIdx[4]])) + 0x28))) << 0x1A) | (0x03FFFFFF & temp_r1_2));
        *var_r5 = (0xFC03FFFF & temp_r1_3) | (((u32)((((u32)(temp_r1_3 << 0xE)) >> 0x17) << 0x18)) >> 6);
        var_r5++;
    } while (var_r6 < 3);
}
