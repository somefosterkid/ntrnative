#include "global.h"

#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_box_funcs.h"

void ov14_021E637C(void *param_1);
void ov14_021E6548(void *param_1, u32 param_2, u32 param_3);
s32 ov14_021E7588(void *param_1, s32 param_2);
void ov14_021F08F0(void *param_1);
void ov14_021F6654(UnkStruct_ov14_boxsys *param_1, s32 param_2);

u32 ov14_021EC7D4(UnkStruct_ov14_boxapp *param_1) {
    MoveSet *iVar1;
    u32 uVar2;

    iVar1 = param_1->sys->unk_C;
    uVar2 = iVar1->from;
    ov14_021F6654(param_1->sys, 0x28);
    PlaySE(0x5ea);
    ov14_021E637C(param_1);
    ov14_021E6548(param_1, iVar1->from, iVar1->to);
    ov14_021F08F0(param_1);
    if (param_1->unk21 == 0xFF) {
        GridInputHandler_SetNextInput(param_1->sys->unk_2C, uVar2 - 0x1e & 0xff);
        GridInputHandler_SetButtonInputMode(param_1->sys->unk_2C, 1);
        ov14_021E7588(param_1, uVar2);
    } else {
        GridInputHandler_SetNextInput(param_1->sys->unk_2C, 8);
        GridInputHandler_SetButtonInputMode(param_1->sys->unk_2C, 1);
    }
    return 0x24;
}
