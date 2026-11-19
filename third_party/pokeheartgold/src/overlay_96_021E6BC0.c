#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

s32 ov96_021E6BC0(PokeathlonCourseData *arg0);

s32 ov96_021E6BC0(PokeathlonCourseData *data) {
    (*(s32(**)(PokeathlonCourseData *, s32))((u8 *)data->state + 0xC))(data, 0);
    if (data->heapAllocPtr4 != NULL) {
        GF_AssertFail();
    }
    PokeathlonCourse_SetStateField07(data, 0xBU);
    return 0;
}
