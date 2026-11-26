#include "global.h"

#include "pokeathlon/pokeathlon.h"

BOOL ov96_021E6FFC(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E6FFC(PokeathlonCourseData *arg0, u8 *unused) {
    u8 *temp_r4;
    s32 temp_r6;

    temp_r4 = PokeathlonCourse_GetDataCopyArea(arg0);
    temp_r6 = ov96_021E9A14();
    if (ov96_021E87B4(0x1E, temp_r4 + 0x28, temp_r6, PokeathlonCourse_GetSystem(arg0)) != 0) {
        PokeathlonCourse_SetStateField07(arg0, 0x26U);
    }
    return 0;
}
