#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_unk_struct_ov96_021_f94a8.h"

void ov96_021FB0F4(PokeathlonCourseData *);

s32 ov96_021FB400(PokeathlonCourseData *);

void ov96_021FAF1C(PokeathlonCourseData *arg0);

void ov96_021FAF1C(PokeathlonCourseData *arg0) {
    u8 *sp8;
    s32 sp4;
    PokeathlonCourseData *sp0;
    s32 temp_r0_2;
    s32 var_r1;
    s32 var_r1_2;
    u32 var_r2_2;
    u16 temp_r0_4;
    u16 temp_r1;
    u8 *temp_r0;
    u8 *var_r0;
    u8 *var_r2_3;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    u8 *var_r2;
#endif
    u8 *var_r3;
    u8 *var_r3_2;
    u8 *var_r4;
    u8 *var_r5;
    u8 *var_r5_2;
    u8 var_r7;
    UnkStruct_ov96_021F94A8 *temp_r5;

    sp0 = arg0;
    temp_r5 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r0 = PokeathlonCourse_GetDataCopyArea(arg0);
    var_r3 = ov96_021E8A20(temp_r0);
#if defined(SDK_PORT) || defined(NONMATCHING)
    for (var_r1 = 0; var_r1 < 3; var_r1++) {
        *(u16 *)var_r3 = temp_r5->rows[var_r1].timer;
        if ((temp_r5->rows[var_r1].unk_8 != 0) && (*(u16 *)((u8 *)var_r3 + 0x10) == 0)) {
            *(u16 *)((u8 *)var_r3 + 0x10) = 1U;
            *(s16 *)((u8 *)var_r3 + 0xA) = (s16)temp_r5->frameCount;
        }
        var_r3 += 2;
    }
#else
    // DS form: the work pointer steps one row (0x6C) per lane, 0xE0 == rows[0].timer, 0xE8 == rows[0].unk_8.
    var_r1 = 0;
    var_r2 = (u8 *)temp_r5;
    do {
        *(u16 *)var_r3 = (u16) * (u16 *)(var_r2 + 0xE0);
        if ((*(var_r2 + 0xE8) != 0) && (*(u16 *)((u8 *)var_r3 + 0x10) == 0)) {
            *(u16 *)((u8 *)var_r3 + 0x10) = 1U;
            *(s16 *)((u8 *)var_r3 + 0xA) = (s16)temp_r5->frameCount;
        }
        var_r1 += 1;
        var_r2 += sizeof(UnkStruct_ov96_021FAB24);
        var_r3 += 2;
    } while (var_r1 < 3);
#endif
    if (ov96_021E5F24(sp0) == 0) {
        var_r7 = 0;
        sp8 = ov96_021E8A20(temp_r0 + 0x28);
        var_r5 = ov96_021E8A20(temp_r0 + 0x50);
        var_r0 = ov96_021E8A20(temp_r0);
        var_r2_2 = 0x12;
        do {
            temp_r1 = *(u16 *)var_r0;
            var_r0 += 2;
            *(u16 *)var_r5 = temp_r1;
            var_r5 += 2;
            var_r2_2 -= 1;
        } while (var_r2_2 != 0);
        var_r5_2 = sp8;
        sp4 = 0;
        var_r4 = temp_r0 + 0x50;
        do {
            u8 *temp_r0_3;
            temp_r0_3 = ov96_021E8A20(var_r4);
            var_r1_2 = 0;
            var_r2_3 = temp_r0_3;
            var_r3_2 = var_r5_2;
loop_10:
            temp_r0_4 = *(u16 *)var_r2_3;
            var_r1_2 += 1;
            var_r2_3 += 2;
            *(u16 *)((u8 *)var_r3_2 + 4) = temp_r0_4;
            var_r3_2 += 2;
            if (var_r1_2 < 3) {
                goto loop_10;
            }
            if (*(u8 *)((u8 *)temp_r0_3 + 9) != 0) {
                var_r7 += 1;
            }
            var_r4 += 0x28;
            var_r5_2 += 6;
            temp_r0_2 = sp4 + 1;
            sp4 = temp_r0_2;
        } while (temp_r0_2 < 4);
        if (var_r7 == PokeathlonCourse_GetParticipantCount(sp0)) {
            *(s32 *)sp8 = 1;
        }
        ov96_021FB400(sp0);
    }
    ov96_021FB0F4(sp0);
}
