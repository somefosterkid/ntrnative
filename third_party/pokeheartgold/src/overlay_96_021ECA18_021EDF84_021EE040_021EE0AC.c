#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_save.h"

#include "save.h"

s32 ov96_021EDF7C(s32);

s32 ov96_021EDF5C(s32);

extern s32 ov96_021EDF84(s32 (*)[], Pokeathlon_UnkSubStruct_B00 *);

extern s32 ov96_021EDFFC(u8 *, s32 (*)[]);

#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov96_021EE040(u8 *, u8 *);
#else
s8 ov96_021EE040(u8 *, u8 *);
#endif

void ov96_021EE0AC(void *arg0, PokeathlonCourseData *arg1);

void ov96_021EE0AC(void *arg0, PokeathlonCourseData *arg1) {
    u8 sp0[32];
    s32 sp20[29] = { 0 };
    Pokeathlon_UnkSubStruct_B00 *sub;
    u32 i;
    u8 *p;
    u8 idx;

    p = sp0 + 1;
    i = 29;
    do {
        *p++ = 0;
        i--;
    } while (i != 0);
    sub = PokeathlonSave_GetAgainUnkB00(Save_Pokeathlon_Get(PokeathlonCourse_GetSaveData(arg1)));
    ov96_021EDF84((s32(*)[]) & sp20[0], sub);
    ov96_021EDFFC(sp0 + 1, (s32(*)[]) & sp20[0]);
    idx = ov96_021EE040(sp0 + 1, sp0);
    *(s32 *)arg0 = (s32)(sp0[0] + ov96_021EDF7C(idx));
    *(s32 *)((u8 *)arg0 + 4) = (s32)((0xFF000000 & *(s32 *)((u8 *)arg0 + 4)) | (0xFFFFFF & sp20[idx]));
    *(s32 *)((u8 *)arg0 + 4) = (s32)((ov96_021EDF5C(sub->unk70) << 0x18) | (0xFFFFFF & *(s32 *)((u8 *)arg0 + 4)));
}
