#include "global.h"
#include "overlay_70_gtsapplication_state.h"
#include "pokedex.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_02241DB4(UnkStruct_ov70_02242E58 *, s32);

s32 ov70_02243400(UnkStruct_ov70_02242E58 *arg0);

s32 ov70_0224342C(u8 *param_1, Pokedex *param_2, s32 param_3, u16 *param_4);

s32 ov70_02243400(UnkStruct_ov70_02242E58 *arg0) {
    s32 v0;

    ov70_02241DB4(arg0, 5);
    if ((s32) arg0->unk_3E >= 0) {
        v0 = 0xC;
    } else {
        v0 = 7;
    }
    arg0->unk_4C = v0;
    return -1;
}

s32 ov70_02243420(UnkStruct_ov70_02242E58 *arg0);

s32 ov70_02243420(UnkStruct_ov70_02242E58 *arg0) {
    arg0->unk_4C = 8;
    return -1;
}

s32 ov70_0224342C(u8 *param_1, Pokedex *param_2, s32 param_3, u16 *param_4) {
    BOOL BVar1;
    s32 iVar2;
    s32 iVar3;

    iVar3 = 0;
    iVar2 = 0;
    if (0 < param_3) {
        do {
            BVar1 = Pokedex_CheckMonSeenFlag(param_2, *param_4);
            if (BVar1 != 0) {
                iVar2 = iVar2 + 1;
            }
            param_4 = param_4 + 1;
            iVar3 = iVar3 + 1;
        } while (iVar3 < param_3);
    }
    return iVar2;
}
