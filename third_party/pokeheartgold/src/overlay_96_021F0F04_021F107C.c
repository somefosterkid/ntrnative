#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "system.h"
#include "touchscreen.h"
#include "overlay_96_ov96_arena_work.h"

u8 ov96_021F46B4(void *arg0);

s32 ov96_021E65A4(PokeathlonCourseData *);

s32 ov96_021E8228(PokeathlonCourseData *, u32, s32, s32, s32);

s32 ov96_021E8318(PokeathlonCourseData *, s16);

s32 ov96_021EB144(void *, s32);

s32 ov96_021EB63C(void *, s32 arg1);

s32 ov96_021F1614(PokeathlonCourseData *);

void ov96_021F2EFC(Ov96ArenaWork *, s32);

u8 ov96_021F30F8(u32, s32);

s32 ov96_021F107C(PokeathlonCourseData *arg0);

s32 ov96_021F107C(PokeathlonCourseData *arg0) {
    s32 var_r1;
    u8 *temp_r0;
    PokeathlonTouchRecord *temp_r0_2;
    PokeathlonTally *tally;
    u8 temp_r0_3;
    u8 temp_r0_4;
    Ov96ArenaWork *temp_r4;

    temp_r4 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r0 = PokeathlonCourse_GetDataCopyArea(arg0);
    temp_r0_2 = ov96_021E8A20(temp_r0);
    temp_r0 = temp_r0 + 0xF0;
    temp_r0_2->touching = 0;
    tally = ov96_021E8A20(temp_r0);
    if ((u8)((tally->pairs[4].word0 >> 0xC) & 1) != 0) {
        ov96_021EB63C(temp_r4->unk_18, 1);
        ov96_021EB144(temp_r4->spriteSet, 1);
        ov96_021E65A4(arg0);
        temp_r0_3 = ov96_021F46B4(temp_r4->unk_774);
        ov96_021E8318(arg0, temp_r0_3);
        ov96_021F2EFC(temp_r4, temp_r0_3);
        return 1;
    }
    if (temp_r4->unk_72A != 0) {
        temp_r0_4 = temp_r4->unk_727;
        var_r1 = 1;
        if ((u32)temp_r0_4 < 1U) {
            var_r1 = 0;
        } else if ((u32)temp_r0_4 >= 3U) {
            if ((u32)temp_r0_4 < 4U) {
                var_r1 = 2;
            } else {
                var_r1 = 3;
            }
        }
        temp_r4->unk_732 = ov96_021F30F8((u8) temp_r4->unk_732, var_r1);
        temp_r4->unk_72A = 0U;
    }
    if (System_GetTouchNew() != 0) {
        ov96_021E8228(arg0, (u8)ov96_021E5F24(arg0), 3, 0, 1);
    }
    if (System_GetTouchHeld() != 0) {
        temp_r0_2->touchX = (s8)gSystem.touchX;
        temp_r0_2->touchY = (s8)gSystem.touchY;
        temp_r0_2->touching = 1;
    }
    ov96_021F1614(arg0);
    return 0;
}
