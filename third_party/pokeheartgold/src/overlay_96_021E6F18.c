#include "global.h"

#include "pokeathlon/pokeathlon.h"

BOOL ov96_021E6F18(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E6F18(PokeathlonCourseData *data, u8 *unused) {
    PokeathlonCourse_ResetDataCopyArea(data);
    PokeathlonCourse_SetStateField07(data, 0x14U);
    return 0;
}
