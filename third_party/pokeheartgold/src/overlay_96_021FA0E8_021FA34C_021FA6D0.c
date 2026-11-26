#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "system.h"
#include "touchscreen.h"
#include "overlay_96_unk_struct_ov96_021_f94a8.h"

void ov96_021E8228(PokeathlonCourseData *, u32, s32, s32, s32);

void ov96_021FA34C(PokeathlonCourseData *, UnkStruct_ov96_021F94A8 *, UnkStruct_ov96_021FAB24 *, s32 *);

u8 ov96_021FB784(UnkStruct_ov96_021F94A8 *, s32, s32);

s32 ov96_021FA6D0(PokeathlonCourseData *arg0, UnkStruct_ov96_021F94A8 *arg1);

s32 ov96_021FA6D0(PokeathlonCourseData *arg0, UnkStruct_ov96_021F94A8 *arg1) {
    s32 sp10[2];
    s32 spC;
    s32 sp8;
    PokeathlonCourseData *sp4;
    s32 *var_r3;
    s32 temp_r0;
    s32 var_r4;
    u8 var_r7;
    UnkStruct_ov96_021FAB24 *var_r5;
    UnkStruct_ov96_021F94A8 *var_r6;

    sp4 = arg0;
    spC = 0;
    sp8 = -1;
    sp10[0] = -1;
    var_r6 = arg1;
    sp10[1] = spC;
    if (System_GetTouchNew() != 0) {
        ov96_021E8228(sp4, (u8)ov96_021E5F24(sp4), 3, 0, 1);
        temp_r0 = ov96_021FB784(var_r6, gSystem.touchX, gSystem.touchY);
        sp8 = temp_r0;
        if (temp_r0 < 3) {
            sp10[0] = temp_r0;
        }
    }
    var_r7 = 0;
    var_r4 = 0;
    var_r5 = var_r6->rows;
    do {
        var_r3 = NULL;
        if (var_r5->unk_8 == 0) {
            if (sp8 == var_r4) {
                var_r3 = sp10;
            }
        } else {
            var_r7 += 1;
        }
        ov96_021FA34C(sp4, var_r6, var_r5, var_r3);
        var_r4 += 1;
        var_r5 += 1;
    } while (var_r4 < 3);
    reg_G2_BG0OFS = (s32)((var_r6->rows[0].unk_6 << 0x10) & 0x01FF0000);
    reg_G2_BG1OFS = (s32)((var_r6->rows[1].unk_6 << 0x10) & 0x01FF0000);
    reg_G2_BG2OFS = (s32)((var_r6->rows[2].unk_6 << 0x10) & 0x01FF0000);
    reg_G2S_DB_BG0OFS = (s32)((var_r6->rows[0].unk_4 << 0x10) & 0x01FF0000);
    reg_G2S_DB_BG1OFS = (s32)((var_r6->rows[1].unk_4 << 0x10) & 0x01FF0000);
    reg_G2S_DB_BG2OFS = (s32)((var_r6->rows[2].unk_4 << 0x10) & 0x01FF0000);
    if ((u32)var_r7 >= 3U) {
        spC = 1;
    }
    return spC;
}
