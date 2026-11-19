#include "global.h"

#include "pokeathlon/pokeathlon.h"

s32 _s32_div_f(s32, u32);

u8 ov96_021E8448(PokeathlonCourseData *param_1, u32 param_2);

void ov96_021EE1D8(u32 *param_1, PokeathlonCourseData *param_2, u8 param_3);

void ov96_021EE1D8(u32 *param_1, PokeathlonCourseData *param_2, u8 param_3) {
    u8 bVar1;
    PokeathlonParticipantData *pPVar2;
    PokeathlonMonKinds *pbVar3;
    s32 iVar4;
    s32 iVar5;

    iVar5 = 0;
    pPVar2 = PokeathlonCourse_GetParticipantData(param_2, param_3);
    iVar4 = 0;
    do {
        pbVar3 = ov96_021E60D8(param_2, param_3, iVar4);
        iVar4 = iVar4 + 1;
        iVar5 += pbVar3->statIdx[2] + pbVar3->statIdx[0] + pbVar3->statIdx[1] + pbVar3->statIdx[3] + pbVar3->statIdx[4] + 5;
    } while (iVar4 < 3);
    iVar4 = iVar5 / 3;
    iVar4 = iVar4 + -8;
    if (iVar4 > 0xC) {
        iVar4 = 0xC;
    } else {
        if (iVar4 < 0) {
            iVar4 = 0;
        }
    }
    bVar1 = ov96_021E8448(param_2, *(u32 *)pPVar2->field_00 & 0xff);
    *param_1 = bVar1;
    param_1[1] = param_1[1] & 0xff000000;
    param_1[1] = (param_1[1] & 0x00ffffff) | (iVar4 << 0x18);
    return;
}
