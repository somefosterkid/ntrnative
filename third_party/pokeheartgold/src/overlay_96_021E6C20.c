#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

s32 ov96_021E6C20(PokeathlonCourseData *arg0);

s32 ov96_021E6C20(PokeathlonCourseData *data) {
    if (PokeathlonCourse_RunSubStateLoop(data) != 0) {
        PokeathlonCourse_SetStateTransitionType(data, 7U);
        PokeathlonCourse_SetStateField07(data, 0xDU);
    }
    return 0;
}

u32 ov96_021E6C40(PokeathlonCourseData *param_1);

u32 ov96_021E6C40(PokeathlonCourseData *param_1) {
    s32 iVar1;

    if (*(s32 *)param_1->filler_3CC == 0) {
        PokeathlonCourse_SetStateField07(param_1, 0xe);
        return 0;
    }
    if ((param_1->courseState).argsPtr == &param_1->stateInfo) {
        (param_1->courseState).argsPtr = (PokeathlonStateInfo *)param_1->filler_3CC;
        (param_1->courseState).exitFlag = 0;
    }
    iVar1 = (**(s32(**)())(*(u8 **)param_1->filler_3CC + (u32)param_1->filler_3CC[6] * 4))(param_1, param_1->filler_3CC + 5);
    if (iVar1 != 0) {
        PokeathlonCourse_SetField1F4(param_1, 0);
        (param_1->courseState).argsPtr = &param_1->stateInfo;
        (param_1->courseState).exitFlag = 0;
        PokeathlonCourse_SetStateField07(param_1, 0xe);
    }
    return 0;
}

s32 ov96_021E6CB8(PokeathlonCourseData *arg0);

s32 ov96_021E6CB8(PokeathlonCourseData *data) {
    (*(s32(**)(PokeathlonCourseData *, s32))((u8 *)data->state + 0xC))(data, 0);
    if (data->heapAllocPtr4 != NULL) {
        GF_AssertFail();
    }
    PokeathlonCourse_SetStateField07(data, 0xFU);
    return 0;
}
