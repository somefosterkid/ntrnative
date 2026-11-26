#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "heap.h"

BOOL ov96_021E7080(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E7080(PokeathlonCourseData *data, u8 *unused) {
    s32 v0;

    if (GF_heap_c_dummy_return_true(HEAP_ID_POKEATHLON) == 0) {
        GF_AssertFail();
    }
    data->state->funcs[3](data, 0);
    if (data->heapAllocPtr4 != NULL) {
        GF_AssertFail();
    }
    v0 = data->currentParticipantIndex;
    if (v0 < (s32)data->maxParticipants) {
        if (data->state->funcs[2] != NULL) {
            PokeathlonCourse_SetStateField07(data, 0x19U);
        } else {
            PokeathlonCourse_SetStateField07(data, 0x18U);
        }
    } else {
        data->currentParticipantIndex = v0 + 1;
        PokeathlonCourse_SetStateField07(data, 0x1DU);
    }
    return 0;
}

BOOL ov96_021E70F0(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E70F0(PokeathlonCourseData *data, u8 *unused) {
    data->currentParticipantIndex += 1;
    if ((s32)data->currentParticipantIndex >= (s32)data->maxParticipants) {
        PokeathlonCourse_SetStateField07(data, 0x1DU);
    } else {
        PokeathlonCourse_SetStateField07(data, 0xBU);
    }
    return 0;
}
