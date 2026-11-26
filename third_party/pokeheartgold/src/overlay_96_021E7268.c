#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "sound_02004A44.h"

BOOL ov96_021E7268(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E7268(PokeathlonCourseData *data, u8 *unused) {
    data->state->funcs[3](data, 0);
    if (data->heapAllocPtr4 != NULL) {
        GF_AssertFail();
    }
    PokeathlonCourse_SetStateField07(data, 0x18U);
    return 0;
}

extern const PokeathlonEventHandlers ov96_0221A844;

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov96_021EB690(struct PokeathlonCourseData *, int);
BOOL ov96_021EB698(struct PokeathlonCourseData *, int);
BOOL ov96_021EB6A0(struct PokeathlonCourseData *, int);

const PokeathlonEventHandlers ov96_0221A844 = {
    .funcs = { ov96_021EB690, ov96_021EB698, NULL, ov96_021EB6A0 },
    .flag = 0,
};
#endif

extern const PokeathlonStateFunc ov96_0221DA5C[];

BOOL ov96_021E7294(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E7294(PokeathlonCourseData *data, u8 *unused) {
    PokeathlonCourse_ResetDataCopyArea(data);
    data->state = &ov96_0221A844;
    data->field_1ED = 0;
    PokeathlonCourse_InitStateInfo(ov96_0221DA5C, &data->subStateInfo);
    PokeathlonCourse_SetStateField07(data, 0x20U);
    return 0;
}


BOOL ov96_021E72CC(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E72CC(PokeathlonCourseData *data, u8 *unused) {
    u8 *v0;

    if (PokeathlonCourse_RunSubStateLoop(data) != 0) {
        v0 = PokeathlonCourse_GetDataCopyArea(data);
        *(s8 *)((u8 *)v0 + 0x24) = 0;
        *(s8 *)((u8 *)v0 + 0x4C) = 1;
        ov96_021E87B0(PokeathlonCourse_GetSystem(data), 1);
        PokeathlonCourse_SetStateTransitionType(data, 0xCU);
        PokeathlonCourse_SetStateField07(data, 0x21U);
        Sound_SetScene(0);
        Sound_SetSceneAndPlayBGM(0x19U, 0x474U, 0);
    }
    return 0;
}
