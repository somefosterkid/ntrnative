#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_unk_struct_ov96_021_f7934.h"
#include "overlay_96_text_box.h"

u32 ov96_021EEA80(Ov96TextBox *box);

s32 ov96_021F85F4(PokeathlonCourseData *arg0);

s32 ov96_021F85F4(PokeathlonCourseData *arg0) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    UnkStruct_ov96_021F7934 *sp14;
#else
    const void *sp14;
#endif
    u8 *sp10;
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
    u8 temp_r6;
    UnkStruct_ov96_021F7934 *temp_r0;

    temp_r0 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    sp14 = temp_r0;
    temp_r0_2 = PokeathlonCourse_GetDataCopyArea(arg0);
    sp10 = temp_r0_2;
    spC = ov96_021E8A20(temp_r0_2 + 0x28);
    sp8 = ov96_021E8A20(temp_r0_2 + 0xF0);
    temp_r1 = (u8)ov96_021E5F24(arg0);
    temp_r6 = (u8)temp_r0->unk_A4;
    sp4 = 0;
    if (temp_r1 == 0) {
        s32 sp0;
        sp0 = 1;
        *(u8 *)((u8 *)spC + 1) = temp_r6;
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
        if ((s32)PokeathlonCourse_GetParticipantCount(arg0) > 0) {
            var_r5 = sp10 + 0x50;
            do {
                temp_r0_3 = ov96_021E8A20(var_r5);
                if (((u32) * (u8 *)temp_r0_3 < (u32)temp_r6) || (*(u8 *)((u8 *)temp_r0_3 + 1) == 1)) {
                    sp4 = 1;
                }
                if (*(u8 *)((u8 *)temp_r0_3 + 2) == 0) {
                    sp0 = 0;
                }
                var_r5 += 0x28;
                var_r4_2 += 1;
            } while (var_r4_2 < (s32)PokeathlonCourse_GetParticipantCount(arg0));
        }
        if (sp0 != 0) {
            *(s8 *)((u8 *)spC + 2) = 1;
        }
        *(s8 *)((u8 *)spC + 3) = (s8)sp4;
    }
    temp_r0_4 = ov96_021E8A20(sp10);
    *(u8 *)temp_r0_4 = temp_r6;
#if defined(SDK_PORT) || defined(NONMATCHING)
    *(s8 *)((u8 *)temp_r0_4 + 1) = ov96_021EEA80(sp14->unk_18);
    *(s8 *)((u8 *)temp_r0_4 + 2) = (s8)sp14->unk_A8;
#else
    *(s8 *)((u8 *)temp_r0_4 + 1) = ov96_021EEA80(*(void **)((u8 *)sp14 + 0x18));
    *(s8 *)((u8 *)temp_r0_4 + 2) = (s8) * (s32 *)((u8 *)sp14 + 0xA8);
    sp14 += 0xA8;
#endif
    if ((*(u8 *)((u8 *)sp8 + 3) != 0) && ((u32)temp_r6 >= (u32) * (u8 *)((u8 *)sp8 + 1))) {
        return 1;
    }
    return 0;
}
