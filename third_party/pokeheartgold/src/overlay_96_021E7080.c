#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "heap.h"

s32 ov96_021E7080(PokeathlonCourseData *arg0);

s32 ov96_021E7080(PokeathlonCourseData *data) {
    s32 v0;

    if (GF_heap_c_dummy_return_true(HEAP_ID_POKEATHLON) == 0) {
        GF_AssertFail();
    }
    (*(s32(**)(PokeathlonCourseData *, s32))((u8 *)data->state + 0xC))(data, 0);
    if (data->heapAllocPtr4 != NULL) {
        GF_AssertFail();
    }
    v0 = data->currentParticipantIndex;
    if (v0 < (s32)data->maxParticipants) {
        if (*(s32 *)((u8 *)data->state + 8) != 0) {
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

s32 ov96_021E70F0(PokeathlonCourseData *arg0);

s32 ov96_021E70F0(PokeathlonCourseData *data) {
    data->currentParticipantIndex += 1;
    if ((s32)data->currentParticipantIndex >= (s32)data->maxParticipants) {
        PokeathlonCourse_SetStateField07(data, 0x1DU);
    } else {
        PokeathlonCourse_SetStateField07(data, 0xBU);
    }
    return 0;
}
