#include "global.h"

#include "pokeathlon/pokeathlon.h"

s32 ov96_021E6168(PokeathlonCourseData *, s32, s32, s32 *);

s32 ov96_021EECB8(void *, s32 *, s32, void *, s32, s32);

void ov96_0220A254(PokeathlonCourseData *arg0, s32 arg1, s32 arg2);

void ov96_0220A254(PokeathlonCourseData *arg0, s32 arg1, s32 arg2) {
    s32 spC[4];
    s32 sp8;
    void *temp_r4;
    void *temp_r6;

    sp8 = arg2;
    temp_r6 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r4 = temp_r6 + 0x50;
    ov96_021E6168(arg0, arg1, arg2, spC);
    ov96_021EECB8(*(void **)((u8 *)(temp_r4 + (0x70 * arg1)) + 8), spC, *(u16 *)((u8 *)temp_r4 + 0x1C4), *(void **)((u8 *)temp_r6 + 0x50), arg1 << 9, *(s32 *)temp_r6);
}
