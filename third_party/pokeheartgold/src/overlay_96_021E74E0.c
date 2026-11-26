#include "global.h"

#include "pokeathlon/pokeathlon.h"

s32 ov96_021E87EC(s32, PokeathlonFieldData *, s32, Ov96System *);

u32 ov96_021E9A1C(void);

BOOL ov96_021E74E0(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E74E0(PokeathlonCourseData *data, u8 *unused) {
    PokeathlonFieldData *pokeathlonFieldData;
    s32 v0;

    pokeathlonFieldData = PokeathlonCourse_GetFieldData_AtIndex(data, 0);
    v0 = ov96_021E9A1C();
    if (ov96_021E87EC(0x20, pokeathlonFieldData, v0, PokeathlonCourse_GetSystem(data)) != 0) {
        PokeathlonCourse_SetStateField07(data, 0x26U);
    }
    return 0;
}
