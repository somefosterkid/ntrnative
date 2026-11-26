#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

BOOL ov96_021E6C20(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E6C20(PokeathlonCourseData *data, u8 *unused) {
    if (PokeathlonCourse_RunSubStateLoop(data) != 0) {
        PokeathlonCourse_SetStateTransitionType(data, 7U);
        PokeathlonCourse_SetStateField07(data, 0xDU);
    }
    return 0;
}

BOOL ov96_021E6C40(PokeathlonCourseData *param_1, u8 *unused);

BOOL ov96_021E6C40(PokeathlonCourseData *param_1, u8 *unused) {
    s32 iVar1;

    if (param_1->subStateInfo.ptr == NULL) {
        PokeathlonCourse_SetStateField07(param_1, 0xe);
        return 0;
    }
    if ((param_1->courseState).argsPtr == &param_1->stateInfo) {
        (param_1->courseState).argsPtr = &param_1->subStateInfo;
        (param_1->courseState).exitFlag = 0;
    }
    iVar1 = param_1->subStateInfo.ptr[param_1->subStateInfo.stateIndex](param_1, &param_1->subStateInfo.stateArgsBase);
    if (iVar1 != 0) {
        PokeathlonCourse_SetField1F4(param_1, 0);
        (param_1->courseState).argsPtr = &param_1->stateInfo;
        (param_1->courseState).exitFlag = 0;
        PokeathlonCourse_SetStateField07(param_1, 0xe);
    }
    return 0;
}

BOOL ov96_021E6CB8(PokeathlonCourseData *arg0, u8 *unused);

BOOL ov96_021E6CB8(PokeathlonCourseData *data, u8 *unused) {
    data->state->funcs[3](data, 0);
    if (data->heapAllocPtr4 != NULL) {
        GF_AssertFail();
    }
    PokeathlonCourse_SetStateField07(data, 0xFU);
    return 0;
}
