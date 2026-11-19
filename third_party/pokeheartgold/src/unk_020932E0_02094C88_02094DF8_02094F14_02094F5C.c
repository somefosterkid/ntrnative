#include "global.h"

#include "sprite.h"
#include "unk_020932E0.h"

void sub_02094F5C(UnkStruct_020932E0 *param_1, s32 param_2) {
    s32 idx;
    unsigned int slot;
    s32 iVar3;

    iVar3 = param_1->unk_884[param_2].unk_8;
    idx = param_1->unk_8D0[iVar3].unk_8;
    slot = param_1->unk_8D0[iVar3].unk_4;
    Sprite_SetDrawFlag(param_1->unk_8D0[iVar3].unk_0, 0);
    param_1->unk_8D0[iVar3].unk_4 = -1;
    param_1->unk_E--;
    if (idx == param_1->unk_F) {
        Sprite_SetOamMode(param_1->unk_198[slot].unk_0, GX_OAM_MODE_NORMAL);
    } else {
        Sprite_SetMatrix(param_1->unk_8C4, Sprite_GetMatrixPtr(param_1->unk_7E8[param_2].unk_0));
        Sprite_SetDrawFlag(param_1->unk_8C4, 1);
        Sprite_SetAnimCtrlSeq(param_1->unk_8C4, 0x32);
    }
    param_1->unk_884[param_2].unk_C = 0;
    Sprite_SetDrawFlag(param_1->unk_7E8[param_2].unk_0, 0);
    param_1->unk_46A4[param_2].unk_4 = 0;
}
