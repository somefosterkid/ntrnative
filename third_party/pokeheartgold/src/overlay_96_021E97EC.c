#include "global.h"

#include "pokeathlon/pokeathlon.h"

void ov96_021E97EC(s32 arg0, s32 arg1, s32 arg2, PokeathlonCourseData *arg3);

void ov96_021E9820(s32 arg0, u32 arg1, void *arg2, PokeathlonCourseData *arg3);

void ov96_021E97EC(s32 arg0, s32 arg1, s32 arg2, PokeathlonCourseData *data) {
    u8 v0;

    PokeathlonCourse_IncrementField1EF(data);
    v0 = PokeathlonCourse_GetParticipantCount(data);
    if (v0 == PokeathlonCourse_GetField1EF(data)) {
        PokeathlonCourse_ResetField1EF(data);
        PokeathlonCourse_SetStateTransitionType(data, 5U);
        PokeathlonCourse_SetStateField07(data, 7U);
    }
}

void ov96_021E9820(s32 arg0, u32 arg1, void *arg2, PokeathlonCourseData *data) {
    u8 *v0;

    v0 = PokeathlonCourse_GetDataCopyArea(data);
    if (*(u8 *)((u8 *)arg2 + 0x24) != 0) {
        memcpy(v0 + 0xF0, arg2, arg1);
        return;
    }
    memcpy(&(v0 + 0x50)[0x28 * arg0], arg2, arg1);
}
