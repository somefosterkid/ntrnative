#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "sprite.h"
#include "unk_020932E0.h"

void sub_020948C4(UnkStruct_020932E0 *param_1, s32 param_2, s32 param_3) {
    VecFx32 *pVVar1;
    NARC *narc;
    void *ptr;
    s32 iVar3;
    UnkStruct_020932E0_Slot *unaff_r7;

    param_1->unk_4680 = 1;
    param_1->unk_4684 = param_3;
    param_1->unk_469C = param_2;
    if (param_2 == 1) {
        unaff_r7 = param_1->unk_198;
        param_1->unk_46A0 = unaff_r7;
    } else if (param_2 == 2) {
        unaff_r7 = param_1->unk_7E8;
        param_1->unk_46A0 = unaff_r7;
    } else {
        GF_AssertFail();
    }
    pVVar1 = Sprite_GetMatrixPtr(unaff_r7[param_3].unk_0);
    param_1->unk_4688 = *pVVar1;
    Sprite_SetAnimCtrlSeq(param_1->unk_8C0, 0x2e);
    Sprite_SetDrawPriority(param_1->unk_46A0[param_1->unk_4684].unk_0, 3);
    if (param_2 == 1) {
        pVVar1 = Sprite_GetMatrixPtr(param_1->unk_46A0[param_1->unk_4684].unk_0);
        iVar3 = 0;
        do {
            param_1->unk_8D4[iVar3].unk_8 = NULL;
            iVar3 = iVar3 + 1;
        } while (iVar3 < 0x1e);
        narc = NARC_New(NARC_poketool_icongra_poke_icon, param_1->unk_4);
        ptr = Heap_AllocAtEnd(param_1->unk_4, 0x1000);
        sub_02094C08(&param_1->unk_46A0[param_1->unk_4684].unk_4, param_1->unk_8D4, 0, param_1->unk_7B0.unk_0, ptr, narc, 0x2b1);
        Sprite_SetMatrix(param_1->unk_7B0.unk_0, pVVar1);
        Sprite_SetDrawFlag(param_1->unk_7B0.unk_0, 1);
        Heap_Free(ptr);
        NARC_Delete(narc);
        param_1->unk_4644 = sub_02094758;
    }
    return;
}
