#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

void ov96_021EEA94(s32, s32, s32, s32);

void _s32_div_f(s32, u32);

u16 ov96_021E8424(u32 param_1);

void ov96_021EEB84(s32, s32, u32, u32);

s32 ov96_021EEDCC(s32 arg0);

void ov96_021F8BC0(s32 *, s32, s32 *);

void ov96_021F8C04(s32 *, s32, s32 *);

void ov96_021F8C2C(s32 *);

void ov96_021F8C54(s32, u32, s32 *);

s32 *ov96_021F8830(s32 *param_1, s32 param_2, s32 param_3, s32 *param_4);

s32 *ov96_021F8830(s32 *param_1, s32 param_2, s32 param_3, s32 *param_4) {
    PokeathlonParticipantData *pPVar1;
    s32 iVar2;
    s32 sVar3;
    s32 remainder;
    s32 flag;
    s32 *psVar4;

    psVar4 = param_1 + 5 + param_1[0xcc] * 0x12;
    if (psVar4[0xc] != 0) {
        GF_AssertFail();
    }
    pPVar1 = PokeathlonCourse_GetParticipantData((PokeathlonCourseData *)param_1[1], *param_4);
    iVar2 = *(s32 *)pPVar1->field_00;
    if (iVar2 > 0) {
        flag = 1;
    } else {
        flag = 0;
    }
    psVar4[0xe] = param_2;
    psVar4[0xf] = param_3;
    psVar4[0x10] = *param_4;
    *(u16 *)(psVar4 + 0x11) = *(u8 *)((u8 *)param_4 + 9);
    ov96_021F8C04(param_1, param_4[4], psVar4);
    ov96_021F8BC0(param_1, *param_4, psVar4);
    param_1[0xcb] = (u32)(*(u16 *)(psVar4 + 0x11) == 0);
    if (psVar4[0x10] >= 4) {
        GF_AssertFail();
    }
    if (flag != 0) {
        sVar3 = ov96_021EEDCC(ov96_021E8424(*(u32 *)pPVar1->field_00 & 0xff));
        ov96_021EEA94(psVar4[3], sVar3, 0xb, *param_1);
    } else {
        ov96_021EEB84(psVar4[3], param_1[1], (u8)psVar4[0x10], 27);
    }
    ov96_021F8C2C(psVar4);
    ov96_021F8C54(param_1[0xcb], 1, psVar4);
    psVar4[0xc] = 1;
    remainder = (s32)(param_1[0xcc] + 1) % (s32)(5);
    param_1[0xcc] = remainder;
    return psVar4;
}
