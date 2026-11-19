#include "global.h"

#include "item.h"

void ov74_02235728(u32, s32, s32, s32, s32, u32);

void ov74_0223589C(u32, u32);

void *ov74_02235930(u32, void *, u32, u32, u32);

void ov74_02235CE4(void *param_1, u32 param_2, void *param_3, u32 param_4);

void ov74_02235CE4(void *param_1, u32 param_2, void *param_3, u32 param_4) {
    s32 iVar1;
    s32 iVar2;
    s32 iVar3;
    s32 iVar4;
    void *uVar5;
    s32 unaff_r4;

    switch (param_2) {
    case 3:
        unaff_r4 = (s32) * (u32 *)(param_3 + 4);
        break;
    case 8:
        unaff_r4 = 0x1c6;
        break;
    case 9:
        unaff_r4 = 0x1c4;
        break;
    case 10:
        unaff_r4 = 0x1c7;
        break;
    case 0xc:
        unaff_r4 = 0x1d3;
        break;
    case 0xe:
        unaff_r4 = 0x1ba;
        break;
    case 0xf:
        unaff_r4 = 0x1f5;
    }
    iVar1 = GetItemIndexMapping(unaff_r4, 1);
    iVar2 = GetItemIndexMapping(unaff_r4, 2);
    iVar3 = GetItemIconCell();
    iVar4 = GetItemIconAnim();
    ov74_02235728(0x12, iVar1, iVar2, iVar3, iVar4, 1);
    ov74_0223589C(0, 0x100000);
    uVar5 = ov74_02235930(1, *(void **)(param_1 + 0x208), 0x80, 0, 0);
    *(void **)(param_1 + 0x208) = uVar5;
    return;
}
