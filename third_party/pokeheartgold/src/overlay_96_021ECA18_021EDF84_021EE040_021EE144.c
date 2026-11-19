#include "global.h"

#include "pokeathlon/pokeathlon.h"

s32 ov96_021EDF5C(s32);

s32 ov96_021EDF7C(s32);

extern s32 ov96_021EDF84(s32 (*)[], PokeathlonUnkSubStruct_974 *);

extern s32 ov96_021EDFFC(u8 *, s32 (*)[]);

s8 ov96_021EE040(u8 *, u8 *);

void ov96_021EE144(void *arg0, PokeathlonCourseData *arg1, s32 arg2);

void ov96_021EE144(void *arg0, PokeathlonCourseData *arg1, s32 arg2) {
    u8 sp0[32];
    s32 sp20[29] = { 0 };
    PokeathlonUnkSubStruct_974 *temp_r0;
    u32 i;
    u8 *p;
    u8 temp_r4;

    p = sp0 + 1;
    i = 29;
    do {
        *p++ = 0;
        i--;
    } while (i != 0);
    temp_r0 = PokeathlonCourse_GetField974_AtIndex(arg1, arg2);
    ov96_021EDF84((s32(*)[]) & sp20[0], temp_r0);
    ov96_021EDFFC(sp0 + 1, (s32(*)[]) & sp20[0]);
    temp_r4 = ov96_021EE040(sp0 + 1, sp0);
    *(s32 *)arg0 = (s32)(sp0[0] + ov96_021EDF7C(temp_r4));
    *(s32 *)((u8 *)arg0 + 4) = (s32)((0xFF000000 & *(s32 *)((u8 *)arg0 + 4)) | (0xFFFFFF & (&sp20[0])[temp_r4]));
    *(s32 *)((u8 *)arg0 + 4) = (s32)((ov96_021EDF5C(*(s32 *)((u8 *)temp_r0 + 0x70)) << 0x18) | (0xFFFFFF & *(s32 *)((u8 *)arg0 + 4)));
}
