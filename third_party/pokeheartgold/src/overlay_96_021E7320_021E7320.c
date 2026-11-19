#include "global.h"

#include "pokeathlon/pokeathlon.h"

u32 ov96_021E7320(PokeathlonCourseData *param_1, u32 param_2, u32 param_3, u32 param_4);

u32 ov96_021E7320(PokeathlonCourseData *param_1, u32 param_2, u32 param_3, u32 param_4) {
    BOOL BVar2;
    void *pvVar3;
    u8 *puVar5;
    u8 *puVar4;
    s32 iVar7;
    u32 n;
    s32 (*pcVar6)(PokeathlonCourseData *, u8 *);

    if (*(s32 *)param_1->filler_3CC == 0) {
        PokeathlonCourse_SetStateField07(param_1, 0x22);
        return 0;
    }
    if ((param_1->courseState).argsPtr == &param_1->stateInfo) {
        (param_1->courseState).argsPtr = (PokeathlonStateInfo *)param_1->filler_3CC;
        (param_1->courseState).exitFlag = 0;
    }
    pcVar6 = (s32(*)(PokeathlonCourseData *, u8 *))((*(void ***)param_1->filler_3CC)[param_1->filler_3CC[6]]);
    iVar7 = pcVar6(param_1, param_1->filler_3CC + 5);
    if (iVar7 != 0) {
        PokeathlonCourse_SetField1F4(param_1, 0);
        (param_1->courseState).argsPtr = &param_1->stateInfo;
        (param_1->courseState).exitFlag = 0;
        PokeathlonCourse_SetStateField07(param_1, 0x22);
    } else {
        BVar2 = ov96_021E5F24(param_1);
        if (BVar2 == 0) {
            pvVar3 = ov96_021E9A14();
            ov96_021E87B4(0x1b, param_1->dataCopyBuffer1, pvVar3, (s32)param_1->system);
            puVar4 = ov96_021E8A20(param_1->dataCopyBuffer2);
            puVar5 = ov96_021E8A20(param_1->dataCopySource);
            n = 0x28;
            do {
                *puVar4++ = *puVar5++;
            } while (--n != 0);
        } else {
            pvVar3 = ov96_021E9A14();
            ov96_021E87B4(0x1b, param_1->dataCopySource, pvVar3, (s32)param_1->system);
        }
    }
    return 0;
}
