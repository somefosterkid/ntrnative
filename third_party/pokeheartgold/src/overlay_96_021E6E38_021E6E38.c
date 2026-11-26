#include "global.h"

#include "pokeathlon/pokeathlon.h"

BOOL ov96_021E6E38(PokeathlonCourseData *param_1, u8 *unused);

BOOL ov96_021E6E38(PokeathlonCourseData *param_1, u8 *unused) {
    u8 uVar1;
    BOOL BVar2;
    s32 pvVar3;
    u8 *puVar4;
    u8 *puVar5;
    u32 iVar7;

    if (param_1->subStateInfo.ptr == NULL) {
        PokeathlonCourse_SetStateField07(param_1, 0x18);
        return 0;
    }
    if ((param_1->courseState).argsPtr == &param_1->stateInfo) {
        (param_1->courseState).argsPtr = &param_1->subStateInfo;
        (param_1->courseState).exitFlag = 0;
    }
    iVar7 = param_1->subStateInfo.ptr[param_1->subStateInfo.stateIndex](param_1, &param_1->subStateInfo.stateArgsBase);
    if (iVar7 != 0) {
        PokeathlonCourse_SetField1F4(param_1, 0);
        (param_1->courseState).argsPtr = &param_1->stateInfo;
        (param_1->courseState).exitFlag = 0;
        PokeathlonCourse_SetStateTransitionType(param_1, 0x10);
        PokeathlonCourse_SetStateField07(param_1, 0x13);
    } else {
        BVar2 = ov96_021E5F24(param_1);
        if (BVar2 == 0) {
            pvVar3 = ov96_021E9A14();
            ov96_021E87B4(0x1b, param_1->dataCopyBuffer1, pvVar3, param_1->system);
            puVar4 = ov96_021E8A20(param_1->dataCopyBuffer2);
            puVar5 = ov96_021E8A20(param_1->dataCopySource);
            iVar7 = 0x28;
            do {
                uVar1 = *puVar5;
                puVar5 = puVar5 + 1;
                *puVar4 = uVar1;
                puVar4 = puVar4 + 1;
                iVar7 = iVar7 + -1;
            } while (iVar7 != 0);
        } else {
            pvVar3 = ov96_021E9A14();
            ov96_021E87B4(0x1b, param_1->dataCopySource, pvVar3, param_1->system);
        }
    }
    return 0;
}
