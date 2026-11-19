#include "global.h"

#include "pokeathlon/pokeathlon.h"

u32 ov96_021E7190(PokeathlonCourseData *param_1, u32 param_2, u32 param_3, u32 param_4);

u32 ov96_021E7190(PokeathlonCourseData *param_1, u32 param_2, u32 param_3, u32 param_4) {
    u8 uVar1;
    BOOL BVar2;
    void *pvVar3;
    u8 *puVar4;
    u8 *puVar5;
    s32 (*pcVar6)();
    u32 iVar7;

    if (*(s32 *)param_1->filler_3CC == 0) {
        PokeathlonCourse_SetStateField07(param_1, 0x1c);
        return 0;
    }
    if ((param_1->courseState).argsPtr == &param_1->stateInfo) {
        (param_1->courseState).argsPtr = (PokeathlonStateInfo *)param_1->filler_3CC;
        (param_1->courseState).exitFlag = 0;
    }
    pcVar6 = (*(s32(***)())param_1->filler_3CC)[param_1->filler_3CC[6]];
    iVar7 = (*pcVar6)(param_1, param_1->filler_3CC + 5);
    if (iVar7 != 0) {
        PokeathlonCourse_SetField1F4(param_1, 0);
        (param_1->courseState).argsPtr = &param_1->stateInfo;
        (param_1->courseState).exitFlag = 0;
        PokeathlonCourse_SetStateField07(param_1, 0x1c);
    } else {
        BVar2 = ov96_021E5F24(param_1);
        if (BVar2 == 0) {
            pvVar3 = ov96_021E9A14();
            ov96_021E87B4(0x1b, param_1->dataCopyBuffer1, pvVar3, (s32)param_1->system);
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
            ov96_021E87B4(0x1b, param_1->dataCopySource, pvVar3, (s32)param_1->system);
        }
    }
    return 0;
}
