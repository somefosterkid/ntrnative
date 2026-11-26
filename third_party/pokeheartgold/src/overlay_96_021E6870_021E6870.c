#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

extern const PokeathlonEventHandlers ov96_0221A8BC;

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov96_021EEE0C(struct PokeathlonCourseData *, int);
BOOL ov96_021EEEBC(struct PokeathlonCourseData *, int);
BOOL ov96_021EEEC0(struct PokeathlonCourseData *, int);

const PokeathlonEventHandlers ov96_0221A8BC = {
    .funcs = { ov96_021EEE0C, ov96_021EEEBC, NULL, ov96_021EEEC0 },
    .flag = 0,
};
#endif

extern const PokeathlonStateFunc ov96_0221DA68[];

BOOL ov96_021E6870(PokeathlonCourseData *param_1, u8 *param_2);

BOOL ov96_021E6870(PokeathlonCourseData *param_1, u8 *param_2) {
    BOOL BVar1;
    s32 iVar2;

    switch (*param_2) {
    case 0:
        param_1->state = &ov96_0221A8BC;
        param_1->field_1ED = 0;
        PokeathlonCourse_InitStateInfo(ov96_0221DA68, &param_1->subStateInfo);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        BVar1 = PokeathlonCourse_RunSubStateLoop(param_1);
        if (BVar1 != 0) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 2:
        if (param_1->subStateInfo.ptr == NULL) {
            *param_2 = *param_2 + 1;
        } else {
            if ((param_1->courseState).argsPtr == &param_1->stateInfo) {
                (param_1->courseState).argsPtr = &param_1->subStateInfo;
                (param_1->courseState).exitFlag = 0;
            }
            iVar2 = param_1->subStateInfo.ptr[param_1->subStateInfo.stateIndex](param_1, &param_1->subStateInfo.stateArgsBase);
            if (iVar2 != 0) {
                PokeathlonCourse_SetField1F4(param_1, 0);
                (param_1->courseState).argsPtr = &param_1->stateInfo;
                (param_1->courseState).exitFlag = 0;
                *param_2 = *param_2 + 1;
            }
        }
        break;
    case 3:
        param_1->state->funcs[3](param_1, 0);
        if (param_1->heapAllocPtr4 != NULL) {
            GF_AssertFail();
        }
        PokeathlonCourse_SetStateField07(param_1, 3);
    }
    return 0;
}
