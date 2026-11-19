#include "global.h"

#include "pokeathlon/pokeathlon.h"

s32 ov96_021E6F18(PokeathlonCourseData *arg0);

s32 ov96_021E6F18(PokeathlonCourseData *data) {
    PokeathlonCourse_ResetDataCopyArea(data);
    PokeathlonCourse_SetStateField07(data, 0x14U);
    return 0;
}
