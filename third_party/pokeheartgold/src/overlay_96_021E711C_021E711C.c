#include "global.h"

#include "pokeathlon/pokeathlon.h"

extern const PokeathlonEventHandlers ov96_0221A86C;

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov96_021F7934(struct PokeathlonCourseData *, int);
BOOL ov96_021F7C4C(struct PokeathlonCourseData *, int);
BOOL ov96_021F7C70(struct PokeathlonCourseData *, int);

const PokeathlonEventHandlers ov96_0221A86C = {
    .funcs = { ov96_021F7934, ov96_021F7C4C, NULL, ov96_021F7C70 },
    .flag = 0,
};
#endif

extern const PokeathlonStateFunc ov96_0221DC24[];

BOOL ov96_021E711C(PokeathlonCourseData *param_1, u8 *unused);

BOOL ov96_021E711C(PokeathlonCourseData *param_1, u8 *unused) {
    param_1->state = &ov96_0221A86C;
    param_1->field_1ED = 0;
    PokeathlonCourse_InitStateInfo(ov96_0221DC24, &param_1->subStateInfo);
    PokeathlonCourse_SetStateField07(param_1, 0x1a);
    return 0;
}
