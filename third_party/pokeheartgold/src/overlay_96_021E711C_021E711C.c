#include "global.h"

#include "pokeathlon/pokeathlon.h"

extern u32 ov96_0221A86C;

extern u32 ov96_0221DC24;

u32 ov96_021E711C(PokeathlonCourseData *param_1);

u32 ov96_021E711C(PokeathlonCourseData *param_1) {
    param_1->state = (u32)&ov96_0221A86C;
    param_1->field_1ED = 0;
    PokeathlonCourse_InitStateInfo(&ov96_0221DC24, (PokeathlonStateInfo *)param_1->filler_3CC);
    PokeathlonCourse_SetStateField07(param_1, 0x1a);
    return 0;
}
