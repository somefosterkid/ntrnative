#include "global.h"

#include "pokeathlon/pokeathlon.h"

extern const PokeathlonEventHandlers ov96_0221A8E4;

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov96_021EF0FC(struct PokeathlonCourseData *, int);
BOOL ov96_021EF18C(struct PokeathlonCourseData *, int);
BOOL ov96_021EF19C(struct PokeathlonCourseData *, int);

const PokeathlonEventHandlers ov96_0221A8E4 = {
    .funcs = { ov96_021EF0FC, ov96_021EF18C, NULL, ov96_021EF19C },
    .flag = 0,
};
#endif

extern const PokeathlonStateFunc ov96_0221DA6C[];

BOOL ov96_021E6BEC(PokeathlonCourseData *param_1, u8 *unused);

BOOL ov96_021E6BEC(PokeathlonCourseData *param_1, u8 *unused) {
    param_1->state = &ov96_0221A8E4;
    param_1->field_1ED = 0;
    PokeathlonCourse_InitStateInfo(ov96_0221DA6C, &param_1->subStateInfo);
    PokeathlonCourse_SetStateField07(param_1, 0xc);
    return 0;
}
