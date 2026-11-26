#include "global.h"
#include "overlay_99_pokeathlon_course_record.h"
#include "sprite_system.h"
struct PokeathlonCourseRecord;

s32 ov98_0221E5C0(void *);

s32 ov98_0221E5D0(void *);

s32 ov98_0221E784(s32, s32, void *, s32);

extern s32 ov99_021E9650;

void ov99_021E6400(struct PokeathlonCourseRecord *arg0);

void ov99_021E6400(PokeathlonCourseRecord *arg0) {
    u32 v0;
    s32 v1;
    u8 v2;

    v1 = ov98_0221E5C0(*((void **)&arg0->spriteContext));
    v0 = ov98_0221E5D0(*((void **)&arg0->spriteContext));
    v2 = 0;
    do {
        ov98_0221E784(v1, v0, &ov99_021E9650 + v2 * 4, 1);
        v2 += 1;
    } while ((u32)v2 < 3U);
}
