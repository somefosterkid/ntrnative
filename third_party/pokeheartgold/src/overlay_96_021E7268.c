#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "sound_02004A44.h"

s32 ov96_021E7268(PokeathlonCourseData *arg0);

s32 ov96_021E7268(PokeathlonCourseData *data) {
    (*(s32(**)(PokeathlonCourseData *, s32))((u8 *)data->state + 0xC))(data, 0);
    if (data->heapAllocPtr4 != NULL) {
        GF_AssertFail();
    }
    PokeathlonCourse_SetStateField07(data, 0x18U);
    return 0;
}

extern s32 ov96_0221A844;

extern s32 ov96_0221DA5C;

s32 ov96_021E7294(PokeathlonCourseData *arg0);

s32 ov96_021E7294(PokeathlonCourseData *data) {
    PokeathlonCourse_ResetDataCopyArea(data);
    data->state = (u32)&ov96_0221A844;
    data->field_1ED = 0;
    PokeathlonCourse_InitStateInfo(&ov96_0221DA5C, (PokeathlonStateInfo *)data->filler_3CC);
    PokeathlonCourse_SetStateField07(data, 0x20U);
    return 0;
}

s32 ov96_021E87B0(void *, s32);

s32 ov96_021E72CC(PokeathlonCourseData *arg0);

s32 ov96_021E72CC(PokeathlonCourseData *data) {
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
