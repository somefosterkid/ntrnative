#include "global.h"

#include "error_handling.h"
#include "unk_02005D10.h"

#include "overlay_96_app_02210C98.h"

void ov96_021EB52C(s16 *param_1, s32 param_2, s32 param_3);

void ov96_021EB564(void *param_1, s32 param_2);

void ov96_02214258(Ov96App_02210C98 *param_1, s32 param_2, s32 param_3, s32 param_4);

void ov96_022123B0(Ov96App_02210C98 *param_1, s32 param_2, s32 param_3, u32 param_4, u8 param_5, u8 param_6);

void ov96_022123B0(Ov96App_02210C98 *param_1, s32 param_2, s32 param_3, u32 param_4, u8 param_5, u8 param_6) {
    s32 remainder;
    u8 uVar1;
    u32 uVar2;
    u32 uVar4;
    s32 iVar5;
    s32 flag;

    uVar1 = (u32)param_5;
    if (uVar1 >= 0xD8) {
        Ov96App_02210C98_Mover *mover = &param_1->unk_62C[param_2];

        if (mover->unk_3F == 0) {
            mover->unk_3F = 1;
            uVar4 = uVar1 - 0xd8 & 0xff;
            if (uVar4 >= 0x10) {
                GF_AssertFail();
            }
            uVar2 = (uVar4 << 22) >> 24;
            ov96_021EB564(param_1->unk_6C4[uVar2], 0xf);
            uVar1 = (s32)uVar4 % 4;
            if (uVar1 != uVar2) {
                flag = 0;
                if (param_3 != 0) {
                    flag = 1;
                }
                ov96_02214258(param_1, uVar1, 1, flag);
                ov96_02214258(param_1, uVar2, 0, 0);
            } else {
                ov96_02214258(param_1, uVar2, 0, 0);
            }
            remainder = (s32)(uVar4) % (s32)(5);
            if (remainder == 0) {
                uVar4 = (s32)uVar4 / 5;
                if (param_6 == uVar4) {
                    PlaySE(0x8c6);
                    return;
                }
            }
            if (param_6 == uVar1) {
                switch (uVar2) {
                case 0:
                case 2:
                    iVar5 = 0x11;
                    break;
                case 1:
                case 3:
                    iVar5 = 0x12;
                    break;
                default:
                    GF_AssertFail();
                    return;
                }
                ov96_021EB52C(param_1->unk_6D4[uVar2], 1, 1);
                ov96_021EB564(param_1->unk_6D4[uVar2], iVar5);
                ov96_021EB52C(param_1->unk_6E4[uVar2], 1, 1);
                ov96_021EB564(param_1->unk_6E4[uVar2], uVar2 + 0x13);
                PlaySE(0x8cb);
                return;
            }
            PlaySE(0x8c6);
        }
    }
    return;
}
