#include "global.h"

#include "sprite.h"

struct PokeathlonCourseData;

s32 ov96_021E6168(struct PokeathlonCourseData *, s32, s32, s32 *);
s32 ov96_021EEBE4(ManagedSprite *, s32 *, const void *, void *, enum HeapID);

void ov96_02219DA8(struct PokeathlonCourseData *arg0, enum HeapID arg1, ManagedSprite *arg2, s32 arg3, u8 arg4);

void ov96_02219DA8(struct PokeathlonCourseData *arg0, enum HeapID arg1, ManagedSprite *arg2, s32 arg3, u8 arg4) {
    s32 sp4[4];

    ov96_021E6168(arg0, arg3, arg4, &sp4[0]);
    ov96_021EEBE4(arg2, &sp4[0], 0, 0, arg1);
}
