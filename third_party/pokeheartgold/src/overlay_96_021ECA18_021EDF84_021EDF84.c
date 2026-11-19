#include "global.h"

void ov96_021EDF84(u32 *param_1, u8 *param_2);

void ov96_021EDF84(u32 *param_1, u8 *param_2) {
    s32 iVar1;
    s32 iVar2;
    u32 *puVar5;
    u32 *puVar3;
    u8 *iVar4;
    iVar1 = (iVar2 = 0);
    iVar4 = param_2;
    do {
        iVar1 = iVar1 + 1;
        iVar2 = iVar2 + (*((s32 *)(iVar4 + 0x44)));
        iVar4 = iVar4 + 4;
    } while (iVar1 < 10);
    *param_1 = *((u32 *)(param_2 + 4));
    param_1[1] = *((u32 *)(param_2 + 8));
    param_1[2] = *((u32 *)(param_2 + 0xc));
    param_1[3] = iVar2;
    param_1[4] = *((u32 *)(param_2 + 0x6c));
    param_1[5] = *((u32 *)(param_2 + 0x10));
    param_1[6] = *((u32 *)(param_2 + 0x14));
    param_1[7] = *((u32 *)(param_2 + 0x18));
    param_1[8] = *((u32 *)(param_2 + 0x1c));
    param_1[9] = *((u32 *)(param_2 + 0x20));
    param_1[10] = *((u32 *)(param_2 + 0x24));
    param_1[0xb] = *((u32 *)(param_2 + 0x28));
    param_1[0xc] = *((u32 *)(param_2 + 0x2c));
    param_1[0xd] = *((u32 *)(param_2 + 0x30));
    param_1[0xe] = *((u32 *)(param_2 + 0x38));
    param_1[0xf] = *((u32 *)(param_2 + 0x3c));
    param_1[0x10] = *((u32 *)(param_2 + 0x40));
    iVar1 = 0;
    puVar3 = param_1;
    puVar5 = (u32 *)param_2;
    do {
        iVar1 = iVar1 + 1;
        puVar3[0x11] = puVar5[0x11];
        puVar5 = puVar5 + 1;
        puVar3 = puVar3 + 1;
    } while (iVar1 < 10);
    param_1[0x1b] = *((u32 *)(param_2 + 0x34));
    param_1[0x1c] = *((u32 *)(param_2 + 0x70));
    return;
}
