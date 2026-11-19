#include "global.h"

#include "pokeathlon/pokeathlon.h"

extern u8 ov96_0221A8E4[];

extern u8 ov96_0221DA6C[];

u32 ov96_021E6BEC(PokeathlonCourseData *param_1);

u32 ov96_021E6BEC(PokeathlonCourseData *param_1) {
    param_1->state = (u32)ov96_0221A8E4;
    param_1->field_1ED = 0;
    PokeathlonCourse_InitStateInfo((void *)ov96_0221DA6C, (PokeathlonStateInfo *)param_1->filler_3CC);
    PokeathlonCourse_SetStateField07(param_1, 0xc);
    return 0;
}
