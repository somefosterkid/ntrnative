#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "sound_02004A44.h"

extern const PokeathlonEventHandlers ov96_0221A808;

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov96_021EB678(struct PokeathlonCourseData *, int);
BOOL ov96_021EB680(struct PokeathlonCourseData *, int);
BOOL ov96_021EB688(struct PokeathlonCourseData *, int);

const PokeathlonEventHandlers ov96_0221A808 = {
    .funcs = { ov96_021EB678, ov96_021EB680, NULL, ov96_021EB688 },
    .flag = 0,
};
#endif

extern const PokeathlonStateFunc ov96_0221DA50[];

BOOL ov96_021E6A54(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E6A54(PokeathlonCourseData *data, u8 *unused) {
    data->counterEnabled = 1;
    PokeathlonCourse_ResetDataCopyArea(data);
    data->state = &ov96_0221A808;
    data->field_1ED = 0;
    PokeathlonCourse_InitStateInfo(ov96_0221DA50, &data->subStateInfo);
    PokeathlonCourse_SetStateField07(data, 8U);
    return 0;
}


BOOL ov96_021E6A94(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E6A94(PokeathlonCourseData *data, u8 *unused) {
    u8 *v0;

    if (PokeathlonCourse_RunSubStateLoop(data) != 0) {
        v0 = PokeathlonCourse_GetDataCopyArea(data);
        *(s8 *)((u8 *)v0 + 0x24) = 0;
        *(s8 *)((u8 *)v0 + 0x4C) = 1;
        ov96_021E87B0(PokeathlonCourse_GetSystem(data), 1);
        PokeathlonCourse_SetStateTransitionType(data, 6U);
        PokeathlonCourse_SetStateField07(data, 9U);
        Sound_SetScene(0);
        Sound_SetSceneAndPlayBGM(0x19U, 0x46FU, 0);
    }
    return 0;
}
