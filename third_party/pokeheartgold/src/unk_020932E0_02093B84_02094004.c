#include "global.h"

#include "sprite.h"
#include "unk_020932E0.h"

void sub_02094004(UnkStruct_020932E0 *param_1) {
    Sprite *uVar1;
    s32 iVar3;
    s32 iVar4;

    sub_0209428C(&param_1->unk_7E4[0x1E], 0xe0, 0xb0, 0x32, 0x20);
    uVar1 = sub_0209417C(param_1, 0xe0, 0xb0, 4, 1);
    param_1->unk_8B4 = uVar1;
    Sprite_SetPriority(param_1->unk_8B4, 3);
    if (param_1->unk_13 == 0) {
        Sprite_SetDrawFlag(param_1->unk_8B4, 0);
    }
    sub_0209428C(&param_1->unk_7E4[0x20], 0x90, 0x10, 0x20, 0x20);
    uVar1 = sub_0209417C(param_1, 0x90, 0x10, 2, 1);
    param_1->unk_8B8 = uVar1;
    sub_0209428C(&param_1->unk_7E4[0x1F], 0x10, 0x10, 0x20, 0x20);
    uVar1 = sub_0209417C(param_1, 0x10, 0x10, 0, 1);
    param_1->unk_8BC = uVar1;
    uVar1 = sub_0209417C(param_1, 0, 0, 0x32, 0);
    param_1->unk_8C4 = uVar1;
    param_1->unk_7E4[param_1->unk_D + 0x21].rect.top = TOUCHSCREEN_RECTLIST_END;
    iVar4 = 0;
    if ((s32)param_1->unk_D > 0) {
        iVar3 = 0;
        do {
            uVar1 = sub_0209417C(param_1, iVar3, 0x40, 6, 0);
            param_1->unk_8D0[iVar4].unk_0 = uVar1;
            param_1->unk_8D0[iVar4].unk_4 = -1;
            param_1->unk_8D0[iVar4].unk_8 = param_1->unk_10;
            Sprite_SetDrawPriority(param_1->unk_8D0[iVar4].unk_0, 4);
            iVar3 = iVar3 + 0x24;
            iVar4 = iVar4 + 1;
        } while (iVar4 < (s32)param_1->unk_D);
    }
    param_1->unk_E = 0;
    uVar1 = sub_0209417C(param_1, 0x14, 0x28, 0x2d, 1);
    param_1->unk_8C0 = uVar1;
    Sprite_SetPriority(param_1->unk_8C0, 1);
    Sprite_SetDrawPriority(param_1->unk_8C0, 1);
    return;
}
