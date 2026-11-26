#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

void ov96_0220D630(UnkStruct_ov96_0220E3F0_Unit *);

void ov96_0220D6B0(UnkStruct_ov96_0220E3F0_Unit *, s16 *);

s32 ov96_0220D8C4(UnkStruct_ov96_0220E3F0_Unit *, s32, s32);

void ov96_0220D9A4(UnkStruct_ov96_0220E3F0_Unit *, PokeathlonCourseData *);

s8 ov96_0220DAA0(UnkStruct_ov96_0220E3F0_Unit *, s16, s32);

s32 ov96_0220F710(PokeathlonCourseData *);

void ov96_0220F8C8(UnkStruct_ov96_0220E3F0_Entry *arg0, PokeathlonCourseData *arg1);

void ov96_0220F8C8(UnkStruct_ov96_0220E3F0_Entry *arg0, PokeathlonCourseData *arg1) {
    s32 sp0;
    s32 temp_r0_5;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r2;
    s32 var_r6;
    u32 temp_r1_2;
    u8 temp_r0_2;
    u8 temp_r2;
    UnkStruct_ov96_0220E3F0_Unit *temp_r0_3;
    UnkStruct_ov96_0220E3F0_Unit *temp_r4;
    UnkStruct_ov96_0220E3F0_Unit *temp_r4_2;
    UnkStruct_ov96_0220E3F0_Unit *var_r4;
    UnkStruct_ov96_0220E3F0_Entry *var_r5;
    var_r5 = arg0;
    ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg1) + 0x28);
    sp0 = 0;
loop_1:
    var_r5->units[var_r5->unk_4.raw >> 0x1E].unk_40.bits.unk_27 = 0;
    temp_r1_2 = var_r5->unk_4.raw;
    if (var_r5->unk_4.bits.b14 != 0) {
        temp_r0_2 = ov96_0220DAA0(var_r5->units, var_r5->unk_0, var_r5->unk_2);
        if (temp_r0_2 != 0xC) {
            var_r5->unk_4.raw = (u32)((temp_r0_2 << 0x1E) | (0x3FFFFFFF & (var_r5->unk_4.raw | 0x20000000)));
        }
    } else if (var_r5->unk_4.bits.b15 != 0) {
        if (var_r5->unk_4.bits.b29 == 1) {
            temp_r4 = &var_r5->units[temp_r1_2 >> 0x1E];
            var_r0 = (s32)(temp_r4->unk_1C.x << 4) >> 16;
            var_r2 = (s32)(temp_r4->unk_1C.y << 4) >> 16;
            if (temp_r4->state == 3) {
                var_r5->unk_4.raw = (u32)(0xDFFFFFFF & temp_r1_2);
            } else {
                var_r0 = var_r5->unk_0 - var_r0;
                if (var_r0 < 0) {
                    var_r0 = 0 - var_r0;
                }
                if (var_r0 <= 8) {
                    var_r0_2 = var_r5->unk_2 - var_r2;
                    if (var_r0_2 < 0) {
                        var_r0_2 = 0 - var_r0_2;
                    }
                    if (var_r0_2 > 8) {
                        goto block_14;
                    }
                } else {
block_14:
                    ov96_0220D6B0(temp_r4, &var_r5->unk_0);

                    temp_r4->unk_40.raw = (s32)((((u32)(ov96_0220D8C4(temp_r4, temp_r4->unk_10.x, temp_r4->unk_10.y) << 0x1F)) >> 2) | (0xDFFFFFFF & temp_r4->unk_40.raw));
                }
            }
        }
    } else {
        if (var_r5->unk_4.bits.b29 == 1) {
            temp_r0_3 = var_r5->units;
            temp_r4_2 = &temp_r0_3[temp_r1_2 >> 0x1E];
            temp_r2 = ov96_0220DAA0(temp_r0_3, var_r5->unk_0, var_r5->unk_2);
            if (temp_r4_2->state == 2) {
                if ((temp_r2 == var_r5->unk_4.bits.sel) && (var_r5->unk_4.bits.v16 <= 5)) {
                    ov96_0220D630(temp_r4_2);
                }
            }
        }
        var_r5->unk_4.raw = (u32)(0xDFFFFFFF & var_r5->unk_4.raw);
    }
    var_r6 = 0;
    var_r4 = var_r5->units;
    do {
        ov96_0220D9A4(var_r4, arg1);
        var_r6 += 1;
        var_r4++;
    } while (var_r6 < 3);
    var_r5++;
    temp_r0_5 = sp0 + 1;
    sp0 = temp_r0_5;
    if (temp_r0_5 < 4) {
        goto loop_1;
    }
    ov96_0220F710(arg1);
}
