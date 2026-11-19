#include "global.h"

#include "error_handling.h"
#include "math_util.h"
#include "overlay_96_app_02210C98.h"

void _s32_div_f(u16, u32);

extern u32 ov96_0221D678;

extern u32 ov96_0221D67A;

s32 ov96_02214DBC(Ov96App_02210C98_Unit *param_1, Ov96App_02210C98_Mover *param_2, s32 param_3, s32 param_4);

s32 ov96_02214DBC(Ov96App_02210C98_Unit *param_1, Ov96App_02210C98_Mover *param_2, s32 param_3, s32 param_4) {
    u16 uVar1;
    u32 uVar2;
    u32 remainder;
    u32 uVar3;
    s32 iVar4;

    uVar1 = LCRandom();
    remainder = (s32)(uVar1) % (s32)(100);
    uVar2 = remainder & 0xff;
    switch (param_3) {
    case 2:
        if (uVar2 < 0x3c) {
            return 0;
        }
        if (uVar2 < 0x50) {
            uVar3 = param_2->unk_8.y;
            param_1->unk_24 = param_2->unk_8;
            return 10;
        }
        uVar3 = param_2->unk_8.y;
        param_1->unk_24 = param_2->unk_8;
        param_1->unk_78 = 1;
        return 10;
    case 3:
        if (uVar2 < 0x14) {
            return 0;
        }
        if (uVar2 < 0x5a) {
            uVar3 = param_2->unk_8.y;
            param_1->unk_24 = param_2->unk_8;
            return 3;
        }
        uVar3 = param_2->unk_8.y;
        param_1->unk_24 = param_2->unk_8;
        param_1->unk_78 = 1;
        return 3;
    case 4:
    case 0xd:
        uVar3 = param_2->unk_8.y;
        param_1->unk_24 = param_2->unk_8;
        param_1->unk_78 = 1;
        break;
    case 1:
    case 9:
        if (param_4 != 4) {
            iVar4 = (3U - param_4 & 0xff) * 0xc;
            param_1->unk_24.x = (u32) * (u16 *)((u8 *)&ov96_0221D678 + iVar4) << 0xc;
            param_1->unk_24.y = (u32) * (u16 *)((u8 *)&ov96_0221D67A + iVar4) << 0xc;
        } else {
            GF_AssertFail();
        }
        break;
    case 0:
    case 8:
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
