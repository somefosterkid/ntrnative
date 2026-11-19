#include "global.h"

#include "pokeathlon/pokeathlon.h"

s32 ov96_021E87EC(s32, PokeathlonParticipantData *, s32, void *);

s32 ov96_021E99FC(void);

s32 ov96_021E6990(PokeathlonCourseData *arg0);

s32 ov96_021E6990(PokeathlonCourseData *data) {
    PokeathlonParticipantData *v0;
    s32 v1;

    v0 = PokeathlonCourse_GetParticipantData(data, 0);
    v1 = ov96_021E99FC();
    if (ov96_021E87EC(0x18, v0, v1, PokeathlonCourse_GetSystem(data)) != 0) {
        PokeathlonCourse_SetStateField07(data, 0x26U);
    }
    return 0;
}
