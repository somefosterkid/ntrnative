#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

BOOL ov96_021E6BC0(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E6BC0(PokeathlonCourseData *data, u8 *unused) {
    data->state->funcs[3](data, 0);
    if (data->heapAllocPtr4 != NULL) {
        GF_AssertFail();
    }
    PokeathlonCourse_SetStateField07(data, 0xBU);
    return 0;
}
