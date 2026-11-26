#include "global.h"

#include "pokeathlon/pokeathlon.h"


BOOL ov96_021E7150(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E7150(PokeathlonCourseData *data, u8 *unused) {
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
