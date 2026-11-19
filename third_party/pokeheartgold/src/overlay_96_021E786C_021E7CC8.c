#include "global.h"

void ov96_021E7CC8(void *param_1, u16 *param_2, u16 *param_3);

void ov96_021E7CC8(void *param_1, u16 *param_2, u16 *param_3) {
    u16 *puVar1;
    s32 iVar2;
    u16 *puVar3;
    s32 iVar4;
    u16 *puVar5;
    u16 *puVar6;

    iVar2 = 0;
    puVar3 = param_2;
    do {
        iVar4 = 0;
        *param_2 = *param_3;
        puVar5 = param_3;
        puVar6 = param_2;
        do {
            puVar1 = puVar5 + 1;
            iVar4 = iVar4 + 1;
            puVar6[1] = *puVar1;
            puVar5 = puVar5 + 1;
            puVar6 = puVar6 + 1;
        } while (iVar4 < 3);
        iVar4 = 0;
        *(u32 *)(puVar3 + 0x16) = *(u32 *)(param_3 + 4);
        puVar5 = param_3;
        puVar6 = puVar3;
        do {
            puVar1 = puVar5 + 6;
            iVar4 = iVar4 + 1;
            puVar6[0x18] = *puVar1;
            puVar5 = puVar5 + 1;
            puVar6 = puVar6 + 1;
        } while (iVar4 < 8);
        iVar2 = iVar2 + 1;
        *(u8 *)(puVar3 + 0x20) = *(u8 *)(param_3 + 0xe);
        param_3 = param_3 + 0x10;
        param_2 = param_2 + 4;
        puVar3 = puVar3 + 0xc;
    } while (iVar2 < 5);
    return;
}
