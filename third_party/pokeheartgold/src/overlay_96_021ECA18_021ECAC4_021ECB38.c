#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "overlay_96_app_021ed1d0.h"

u16 ov96_021E8424(u32 param_1);

void ov96_021EEA88(ManagedSprite *param_1, u32 param_2, s32 param_3, s32 param_4);

void ov96_021EEB74(ManagedSprite **param_1, PokeathlonCourseData *param_2, s32 param_3, s32 param_4);

u32 ov96_021EEDCC(s32 param_1);

void ov96_021ECB38(ManagedSprite **param_1, PokeathlonCourseData *param_2, u32 param_3, s32 param_4, s32 param_5);

void ov96_021ECB38(ManagedSprite **param_1, PokeathlonCourseData *param_2, u32 param_3, s32 param_4, s32 param_5) {
    u16 uVar1;
    u32 uVar2;
    PokeathlonParticipantData *pPVar2;
    s32 iVar3;
    s32 val;
    int flag;

    pPVar2 = PokeathlonCourse_GetParticipantData(param_2, param_3);
    iVar3 = *(s32 *)pPVar2->field_00;
    if (iVar3 > 0) {
        flag = 1;
    } else {
        flag = 0;
    }
    switch (param_4) {
    case 1:
        val = 0;
        break;
    case 2:
        val = flag ? 0xb : 0x1b;
        break;
    default:
        GF_AssertFail();
        break;
    }
    if (flag) {
        pPVar2 = PokeathlonCourse_GetParticipantData(param_2, param_3);
        uVar1 = ov96_021E8424(*(u32 *)pPVar2->field_00 & 0xff);
        uVar2 = ov96_021EEDCC(uVar1);
        ov96_021EEA88(param_1[param_3], uVar2, (u8)val, param_5);
    } else {
        ov96_021EEB74(param_1, param_2, (u8)param_3, val);
    }
}
