#include "global.h"

#include "unk_02005D10.h"

void ov40_0222BF80(s32, u32);

void ov40_0222DF60(s32, u32);

void ov40_02230944(s32);

s32 ov40_0223DB94(s32);

void ov40_0223DDE8(s32, int, int);

void ov40_0223DEB8(s32);

u8 sub_02031620(void *);

u8 sub_0203162C(void *);

void ov40_02241D10(u32 param_1, s32 param_2, s32 param_3);

void ov40_02241D10(u32 param_1, s32 param_2, s32 param_3) {
    u8 uVar1;
    s32 iVar2;
    s32 iVar3;
    iVar3 = *((s32 *)(param_3 + 0x860));
    if (param_2 == 0) {
        switch (param_1) {
        case 0:
            ov40_02230944(param_3);
            *((s8 *)(iVar3 + 0x4c5)) = (s8)param_1;
            ov40_0222BF80(param_3, 5);
            return;

        case 1:
            ov40_02230944(param_3);
            *((s8 *)(iVar3 + 0x4c5)) = (s8)param_1;
            ov40_0222BF80(param_3, 5);
            return;

        case 2:
            iVar2 = sub_02031620(*((void **)(param_3 + 0x88c)));
            ov40_02230944(param_3);
            if (iVar2 == 0) {
                PlaySE(0x0000057c);
                ov40_0222DF60(param_3, 128);
                return;
            }
            uVar1 = -1;
            if ((*((u8 *)(iVar3 + 0x4c3))) == uVar1) {
                uVar1 = sub_02031620(*((void **)(param_3 + 0x88c)));
                *((u8 *)(iVar3 + 0x4c3)) = uVar1;
                uVar1 = sub_0203162C(*((void **)(param_3 + 0x88c)));
                *((u8 *)(iVar3 + 0x4c4)) = uVar1;
            } else {
                *((u8 *)(iVar3 + 0x4c3)) = 0xff;
                *((u8 *)(iVar3 + 0x4c4)) = 0xff;
            }
            ov40_0223DDE8(param_3, *((u8 *)(iVar3 + 0x4c3)), *((u8 *)(iVar3 + 0x4c4)));
            ov40_0223DEB8(param_3);
            return;

        case 3:
            ov40_02230944(param_3);
            ov40_0222BF80(param_3, 4);
            return;

        case 4:
            ov40_02230944(param_3);
            iVar3 = ov40_0223DB94(param_3);
            if (iVar3 == 0) {
                PlaySE(0x0000057c);
                ov40_0222DF60(param_3, 0x74);
                return;
            }
            ov40_0222BF80(param_3, 0xc);
        }
    }
    return;
}
