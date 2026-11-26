#include "global.h"
#include "pokeathlon/pokeathlon.h"
#include "sound_chatot.h"
#include "overlay_96_app_021ed1d0.h"

s32 ov96_021EDD64(Ov96App_021ED1D0 *arg0);

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_021EC1B0.c (tools/pokehg_abi.py)
void ov96_021EC298(Ov96App_021ED1D0 *, s32);
#else
s32 ov96_021EC298(Ov96App_021ED1D0 *, u8);
#endif
s32 ov96_021ECC38(Ov96PlayerSlot *, s32, s32);
s32 ov96_021ED5AC(PokeathlonCourseData *, s32, s32);
s32 ov96_021EDDA4(PokeathlonCourseData *arg0, s32 (*arg1)(PokeathlonCourseData *, u8, u8), u32 arg2);

s32 ov96_021EDD64(Ov96App_021ED1D0 *arg0) {
    arg0->monIdx = (u8)(arg0->monIdx + 1);
    if ((u32)arg0->monIdx >= 3U) {
        arg0->monIdx = 0U;
        arg0->playerIdx = (u8)(arg0->playerIdx + 1);
    }
    if (arg0->playerIdx == 4) {
        return 1;
    }
    return 0;
}

s32 ov96_021EDDA4(PokeathlonCourseData *data, s32 (*arg1)(PokeathlonCourseData *, u8, u8), u32 arg2) {
    u8 v1;
    u32 v4;
    u8 *v0;
    Ov96App_021ED1D0 *v2;
    Ov96SpriteResDesc *v3;

    v2 = PokeathlonCourse_GetHeapAllocPtr4(data);
loop_1:
    if (arg1(data, v2->playerIdx, v2->monIdx) == 0) {
        if (ov96_021EDD64(v2) == 0) {
            goto loop_1;
        }
    }
    v1 = v2->playerIdx;
    if ((u32)v1 < 4U) {
        v4 = v2->monIdx + (v1 * 3);
        v0 = v2->revealCount;
        v0[v1] = v0[v1] + 1;
        v2->counter = (u8)(v2->counter + 1);
        ov96_021ED5AC(data, v2->playerIdx, v2->monIdx);
        ov96_021ECC38(v2->playerSlots, v2->playerIdx, (u16)arg2);
        ov96_021EC298(v2, v4);
        v3 = ov96_021E8BB0(ov96_021EAA20(ov96_021EAA04(v2->spriteSet, (u8)v4)));
        sub_02006E3C(1U);
        PlayCry(v3->species, (u8)v3->unk_2);
        ov96_021EDD64(v2);
        return 1;
    }
    return 0;
}
