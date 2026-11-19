#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_app_021ed1d0.h"
#include "overlay_96_text_box.h"

u32 ov96_021EEA80(Ov96TextBox *box);

s32 ov96_021EE324(PokeathlonCourseData *arg0);

s32 ov96_021EE324(PokeathlonCourseData *arg0) {
    Ov96App_021ED1D0 *sp14;
    u8 *sp10;
    Ov96App_021ED1D0 *temp_r0;
    u8 *temp_r0_2;
    u8 *spC;
    u8 *sp8;
    u32 var_r2;
    s32 var_r4_2;
    s32 sp4;
    u8 *temp_r0_3;
    u8 *temp_r0_4;
    u8 *var_r0;
    u8 *var_r4;
    u8 *var_r5;
    u8 temp_r1;
    u8 temp_r1_2;
    u8 temp_r1_3;
    u8 temp_r6;
    s32 sp0;
    temp_r0 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    sp14 = temp_r0;
    temp_r0_2 = PokeathlonCourse_GetDataCopyArea(arg0);
    sp10 = temp_r0_2;
    spC = ov96_021E8A20(temp_r0_2 + 0x28);
    sp8 = ov96_021E8A20(temp_r0_2 + 0xF0);
    temp_r1 = (u8)ov96_021E5F24(arg0);
    temp_r6 = temp_r0->step;
    sp4 = 0;
    if (temp_r1 == 0) {
        sp0 = 1;
        *((u8 *)(((u8 *)spC) + 0x20)) = temp_r6;
        var_r4 = ov96_021E8A20(sp10 + 0x50);
        var_r0 = ov96_021E8A20(sp10);
        var_r2 = 0x24;
        do {
            temp_r1_2 = *var_r0;
            var_r0 += 1;
            *var_r4 = temp_r1_2;
            var_r4 += 1;
            var_r2 -= 1;
        } while (var_r2 != 0);
        var_r4_2 = 0;
        if (((s32)PokeathlonCourse_GetParticipantCount(arg0)) > 0) {
            var_r5 = sp10 + 0x50;
            do {
                temp_r0_3 = ov96_021E8A20(var_r5);
                if ((((u32)(*((u8 *)temp_r0_3))) < ((u32)temp_r6)) || ((*((u8 *)(((u8 *)temp_r0_3) + 1))) == 1)) {
                    sp4 = 1;
                }
                if ((*((u8 *)(((u8 *)temp_r0_3) + 2))) == 0) {
                    sp0 = 0;
                }
                var_r5 += 0x28;
                var_r4_2 += 1;
            } while (var_r4_2 < ((s32)PokeathlonCourse_GetParticipantCount(arg0)));
        }
        if (sp0 != 0) {
            *((u8 *)(((u8 *)spC) + 0x21)) = (u8)((*((u8 *)(((u8 *)spC) + 0x21))) | 2);
        }
        temp_r1_3 = ((*((u8 *)(((u8 *)spC) + 0x21))) & (~1)) | (1 & ((u8)sp4));
        *((u8 *)(((u8 *)spC) + 0x21)) = temp_r1_3;
        spC += 0x21;
    }
    temp_r0_4 = ov96_021E8A20(sp10);
    *((u8 *)temp_r0_4) = temp_r6;
    *((s8 *)(((u8 *)temp_r0_4) + 1)) = ov96_021EEA80(sp14->textBox);
    *((s8 *)(((u8 *)temp_r0_4) + 2)) = (s8)sp14->unk_B8;
    if ((((u32)((*((u8 *)(((u8 *)sp8) + 0x21))) << 0x1F)) >> 0x1F) != 0) {
        if (((u32)temp_r6) >= ((u32)(*((u8 *)(((u8 *)sp8) + 0x20))))) {
            return 1;
        }
    }
    sp8 += 0x20;
    return 0;
}
