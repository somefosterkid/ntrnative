#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

extern const PokeathlonStateHandlerFunc ov96_0221A8BC[];

extern const PokeathlonStateFunc ov96_0221DA68[];

u32 ov96_021E6870(PokeathlonCourseData *param_1, u8 *param_2, u32 param_3, u32 param_4);

u32 ov96_021E6870(PokeathlonCourseData *param_1, u8 *param_2, u32 param_3, u32 param_4) {
    BOOL BVar1;
    s32 iVar2;
    void (*pcVar3)();

    switch (*param_2) {
    case 0:
        param_1->state = (u32)ov96_0221A8BC;
        param_1->field_1ED = 0;
        PokeathlonCourse_InitStateInfo(ov96_0221DA68, (PokeathlonStateInfo *)param_1->filler_3CC);
        *param_2 = *param_2 + 1;
        break;
    case 1:
        BVar1 = PokeathlonCourse_RunSubStateLoop(param_1);
        if (BVar1 != 0) {
            *param_2 = *param_2 + 1;
        }
        break;
    case 2:
        if (*(s32 *)param_1->filler_3CC == 0) {
            *param_2 = *param_2 + 1;
        } else {
            if ((param_1->courseState).argsPtr == &param_1->stateInfo) {
                (param_1->courseState).argsPtr = (PokeathlonStateInfo *)param_1->filler_3CC;
                (param_1->courseState).exitFlag = 0;
            }
            iVar2 = (**(s32(**)())(*(u8 **)param_1->filler_3CC + (u32)param_1->filler_3CC[6] * 4))(param_1, param_1->filler_3CC + 5);
            if (iVar2 != 0) {
                PokeathlonCourse_SetField1F4(param_1, 0);
                (param_1->courseState).argsPtr = &param_1->stateInfo;
                (param_1->courseState).exitFlag = 0;
                *param_2 = *param_2 + 1;
            }
        }
        break;
    case 3:
        pcVar3 = *(void (**)())(param_1->state + 0xc);
        (*pcVar3)(param_1, 0);
        if (param_1->heapAllocPtr4 != NULL) {
            GF_AssertFail();
        }
        PokeathlonCourse_SetStateField07(param_1, 3);
    }
    return 0;
}
