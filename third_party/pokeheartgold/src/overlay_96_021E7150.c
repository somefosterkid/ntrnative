#include "global.h"

#include "pokeathlon/pokeathlon.h"

s32 ov96_021E87B0(void *, s32);

s32 ov96_021E7150(PokeathlonCourseData *arg0);

s32 ov96_021E7150(PokeathlonCourseData *data) {
    u8 *v0;

    if (PokeathlonCourse_RunSubStateLoop(data) != 0) {
        v0 = PokeathlonCourse_GetDataCopyArea(data);
        *(s8 *)((u8 *)v0 + 0x24) = 0;
        *(s8 *)((u8 *)v0 + 0x4C) = 1;
        ov96_021E87B0(PokeathlonCourse_GetSystem(data), 1);
        PokeathlonCourse_SetStateTransitionType(data, 0xBU);
        PokeathlonCourse_SetStateField07(data, 0x1BU);
    }
    return 0;
}
