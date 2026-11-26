#include "global.h"

#include "pokeathlon/pokeathlon.h"

s32 ov96_021E87EC(s32, PlayerProfile *, s32, Ov96System *);

u32 ov96_021E9A04(void);

BOOL ov96_021E69C4(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E69C4(PokeathlonCourseData *data, u8 *unused) {
    s32 v0;

    v0 = 1;
    if (ov96_021E5F24(data) == 0) {
        v0 = ov96_021E87EC(0x19, data->playerProfiles, ov96_021E9A04(), data->system);
    }
    if (v0 != 0) {
        PokeathlonCourse_SetStateField07_IfDifferent(data, 0x26U, 6U);
    }
    return 0;
}
