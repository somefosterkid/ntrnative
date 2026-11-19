#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "sound_02004A44.h"

extern s32 ov96_0221A808;

extern s32 ov96_0221DA50;

s32 ov96_021E6A54(PokeathlonCourseData *arg0);

s32 ov96_021E6A54(PokeathlonCourseData *data) {
    data->counterEnabled = 1;
    PokeathlonCourse_ResetDataCopyArea(data);
    data->state = (u32)&ov96_0221A808;
    data->field_1ED = 0;
    PokeathlonCourse_InitStateInfo(&ov96_0221DA50, (PokeathlonStateInfo *)data->filler_3CC);
    PokeathlonCourse_SetStateField07(data, 8U);
    return 0;
}

s32 ov96_021E87B0(void *, s32);

s32 ov96_021E6A94(PokeathlonCourseData *arg0);

s32 ov96_021E6A94(PokeathlonCourseData *data) {
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
