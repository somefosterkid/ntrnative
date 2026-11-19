#include "global.h"

#include "sprite.h"
#include "overlay_96_ov96_digit_work.h"

struct PokeathlonCourseData;

s32 ov96_021E6168(struct PokeathlonCourseData *, s32, s32, s32 *);

void ov96_021EECB8(ManagedSprite *, s32 *, s32, struct NARC *, s32, enum HeapID);

void ov96_02219DD0(Ov96DigitWork *work, s32 arg1, s32 arg2);

void ov96_02219DD0(Ov96DigitWork *work, s32 arg1, s32 arg2) {
    s32 sp8[4];
    ManagedSprite *temp_r6;
    s32 idx;

    idx = arg1 + 2;
    temp_r6 = work->sprites[idx];
    ov96_021E6168(work->course, arg1, arg2, sp8);
    ov96_021EECB8(temp_r6, sp8, work->plttResId, work->narc, arg1 << 9, work->heapId);
}
