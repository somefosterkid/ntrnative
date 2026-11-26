#include "global.h"

#include "pokeathlon/pokeathlon.h"

BOOL ov96_021F9234(PokeathlonCourseData *param_1, u8 *unused);

BOOL ov96_021F9234(PokeathlonCourseData *param_1, u8 *unused) {
    PokeathlonCourse_SetStateField07(param_1, 1);
    return 0;
}
