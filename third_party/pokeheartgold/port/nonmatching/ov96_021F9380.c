/* PORTOK hk42 */
#include "pokeathlon/pokeathlon.h"
#include "screen_fade.h"
#include "pokeathlon/pokeathlon.h"
#include "screen_fade.h"
s32 ov96_021FBDEC(s32 arg0);
s32 ov96_021F9380(PokeathlonCourseData *arg0, u8 *arg1) {
    f32 sp14;
    void *temp_r5;
    s32 sp10;
    s32 sp8;
    s32 sp4;
    PokeathlonCourseData *sp0;
    f32 temp_r0_3;
    s32 spC;
    s32 temp_r0_2;
    s32 temp_r0_4;
    s32 temp_r0_5;
    s32 temp_r4;
    s32 var_r0;
    s32 var_r2;
    f32 var_r4;
    s32 var_r4_2;
    u32 var_r6;
    u16 temp_r1;
    s32 temp_r0;
    u8 *temp_r7;
    u8 *var_r3;

    sp0 = arg0;
    temp_r7 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    if (*arg1 == 0) {
        if (IsPaletteFadeFinished() != 0) {
            *arg1 = 1;
        }
        return 0;
    }
    temp_r4 = ov96_021E5F24(sp0);
    var_r0 = 0;
    var_r2 = 0;
    var_r3 = temp_r7;
    do {
        temp_r1 = *(u16 *)((u8 *)var_r3 + 0xE2);
        var_r2 += 1;
        var_r3 += 0x6C;
        var_r0 += temp_r1;
    } while (var_r2 < 3);
    PokeathlonCourse_SetField5E0_AtIndex(sp0, (s32) (u8) temp_r4, (u16) ov96_021FBDEC(var_r0));
    if (temp_r4 == 0) {
        temp_r0 = (s32) PokeathlonCourse_GetParticipantCount(sp0);
        sp8 = temp_r0;
        sp10 = temp_r0;
        if (temp_r0 < 4) {
            do {
                sp4 = 0;
                spC = 0;
                var_r6 = (sp10 - sp8) * 3;
loop_9:
                temp_r5 = temp_r7 + (0x28 * var_r6);
                if (*(s32 *)((u8 *)temp_r5 + 0x250) == 0) {
                    var_r4 = *(f32 *)((u8 *)temp_r5 + 0x240);
                    if (var_r4 < *(f32 *)((u8 *)temp_r5 + 0x248)) {
                        var_r4 = *(f32 *)((u8 *)temp_r5 + 0x248);
                    }
                    temp_r0_3 = (f32) (0x1000 - *(s32 *)((u8 *)temp_r5 + 0x258)) / var_r4;
                    sp14 = temp_r0_3;
                    temp_r0_4 = (s32) temp_r0_3;
                    var_r4_2 = temp_r0_4;
                    if ((temp_r0_3 - (f32) temp_r0_4) > 0.0f) {
                        var_r4_2 += 1;
                    }
                    *(s32 *)((u8 *)temp_r5 + 0x254) = (s32) (*(s32 *)((u8 *)temp_r7 + 0x230) + var_r4_2);
                }
                var_r6 += 1;
                sp4 += *(s32 *)((u8 *)temp_r5 + 0x254);
                temp_r0_5 = spC + 1;
                spC = temp_r0_5;
                if (temp_r0_5 < 3) {
                    goto loop_9;
                }
                PokeathlonCourse_SetField5E0_AtIndex(sp0, (s32) (u8) sp10, (u16) ov96_021FBDEC(sp4));
                temp_r0_2 = sp10 + 1;
                sp10 = temp_r0_2;
            } while (temp_r0_2 < 4);
        }
    }
    return 1;
}
