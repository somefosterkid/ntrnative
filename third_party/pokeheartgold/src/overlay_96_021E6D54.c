#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "heap.h"
#include "sound_02004A44.h"


BOOL ov96_021E6D54(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E6D54(PokeathlonCourseData *data, u8 *unused) {
    u8 *v0;

    if (PokeathlonCourse_RunSubStateLoop(data) != 0) {
        v0 = PokeathlonCourse_GetDataCopyArea(data);
        *(s8 *)((u8 *)v0 + 0x24) = 0;
        *(s8 *)((u8 *)v0 + 0x4C) = 1;
        ov96_021E87B0(PokeathlonCourse_GetSystem(data), 1);
        PokeathlonCourse_SetStateTransitionType(data, 8U);
        if (data->currentParticipantIndex == (data->maxParticipants - 1)) {
            Sound_SetScene(0);
            Sound_SetSceneAndPlayBGM(0x18U, 0x472U, 0);
        } else {
            Sound_SetScene(0);
            Sound_SetSceneAndPlayBGM(0x18U, 0x471U, 0);
        }
        sub_020053A8(7U, 1U);
        PokeathlonCourse_SetStateField07(data, 0x11U);
        if (GF_heap_c_dummy_return_true(HEAP_ID_POKEATHLON) == 0) {
            GF_AssertFail();
        }
    }
    return 0;
}

s32 ov96_021E87EC(s32, void *, s32, Ov96System *);

BOOL ov96_021E6DE8(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E6DE8(PokeathlonCourseData *data, u8 *unused) {
    if (data->field_3A4.active != 0U) {
        if (ov96_021E5F24(data) == 0) {
            if (ov96_021E87EC(0x1C, data->field_3A4.sendData, data->field_3A4.size, data->system) != 0) {
                PokeathlonCourse_SetStateField07(data, 0x26U);
            }
        } else {
            PokeathlonCourse_SetStateField07(data, 0x26U);
        }
    } else {
        PokeathlonCourse_SetStateField07(data, 0x12U);
    }
    return 0;
}
