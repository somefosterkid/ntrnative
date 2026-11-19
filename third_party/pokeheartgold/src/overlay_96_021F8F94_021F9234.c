#include "global.h"

#include "pokeathlon/pokeathlon.h"

u32 ov96_021F9234(PokeathlonCourseData *param_1);

u32 ov96_021F9234(PokeathlonCourseData *param_1) {
    PokeathlonCourse_SetStateField07(param_1, 1);
    return 0;
}
