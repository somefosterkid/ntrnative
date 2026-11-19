#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "filesystem.h"
#include "heap.h"
#include "overlay_96_sprite_res_man.h"

s32 ov96_0220BE28(void *, void *, void *, s32, u8, PokeathlonCourseData *, s32 *);

void ov96_0220C844(Ov96SpriteSet *arg0, s32 arg1);

void *ov96_0220B7F4(enum HeapID arg0, void *arg1, void *arg2, PokeathlonCourseData *arg3, Ov96SpriteSet *arg4);

void *ov96_0220B7F4(enum HeapID arg0, void *arg1, void *arg2, PokeathlonCourseData *arg3, Ov96SpriteSet *arg4) {
    s32 var_r6;
    s32 sp20[18];
    const void *sp1C;
    void *sp18;
    s32 sp14;
    void *sp10;
    void *spC;
    s32 var_r4;
    s32 *var_r5;
    u8 temp_r0_2;

    void *temp_r0;
    spC = arg1;
    sp10 = arg2;
    sp14 = (s32)((u8)ov96_021E5F24(arg3));
    ReadWholeNarcMemberByIdPair(&sp20[3], NARC_a_1_7_0, 5);
    temp_r0 = Heap_Alloc(arg0, 0xC8U);
    sp1C = temp_r0;
    MI_CpuFill8(temp_r0, 0U, 0xC8U);
    *((enum HeapID *)temp_r0) = arg0;
    *((void **)(((u8 *)temp_r0) + 4)) = spC;
    *((void **)(((u8 *)temp_r0) + 8)) = sp10;
    *((PokeathlonCourseData **)(((u8 *)temp_r0) + 0xC)) = arg3;
    *((Ov96SpriteSet **)(((u8 *)sp1C) + 0x10)) = arg4;
    ov96_0220C844(arg4, sp14);
    var_r6 = 0;
    sp18 = temp_r0;
    sp18 = temp_r0 + 0x14;
    do {
        if (var_r6 != sp14) {
            var_r4 = 0;
            var_r5 = &sp20[0];
            do {
                temp_r0_2 = ov96_021E60D8(arg3, var_r6, var_r4)->statIdx[3];
                *var_r5++ = *((s32 *)(((u8 *)(&(&sp20[3])[temp_r0_2])) + 0x14));
                var_r4 += 1;
            } while (var_r4 < 3);
            ov96_0220BE28(sp18, spC, sp10, *((s32 *)(((u8 *)sp1C) + 0x10)), var_r6, arg3, &sp20[0]);
            sp18 += 0x3C;
        }
        var_r6 += 1;
    } while (var_r6 < 4);
    return sp1C;
}
