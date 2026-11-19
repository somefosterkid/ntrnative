#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021E7808(void *param_1, s32 param_2, u32 param_3, u32 param_4, s32 param_5);
void ov14_021E783C(u8 *param_1, s32 param_2, s32 param_3);

void ov14_021E783C(u8 *param_1, s32 param_2, s32 param_3) {
    unsigned char new_var;
    s8 cVar1;
    u32 uVar2;
    s32 iVar3;
    u8 *iVar4;
    if (param_3 == 0) {
        *((s8 *)&((UnkStruct_ov14_boxapp *)param_1)->sys->unk_448[0]) = (*((s8 *)&((UnkStruct_ov14_boxapp *)param_1)->sys->unk_448[0])) + (-0x17);
        cVar1 = *((s8 *)&((UnkStruct_ov14_boxapp *)param_1)->sys->unk_448[0]);
        if (cVar1 < 0) {
            *((s8 *)&((UnkStruct_ov14_boxapp *)param_1)->sys->unk_448[0]) = cVar1 + 0x40;
        }
    } else if (param_3 == 1) {
        *((s8 *)&((UnkStruct_ov14_boxapp *)param_1)->sys->unk_448[0]) = (*((s8 *)&((UnkStruct_ov14_boxapp *)param_1)->sys->unk_448[0])) + 0x17;
        cVar1 = *((s8 *)&((UnkStruct_ov14_boxapp *)param_1)->sys->unk_448[0]);
        if ((0x3f + 1) <= cVar1) {
            *((s8 *)&((UnkStruct_ov14_boxapp *)param_1)->sys->unk_448[0]) = cVar1 + (-0x40);
        }
    }
    iVar4 = ((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys);
    new_var = *((s8 *)&((UnkStruct_ov14_boxsys *)iVar4)->unk_448[1]);
    if (new_var == 0) {
        uVar2 = 0x25c;
        iVar3 = 0xe;
    } else {
        uVar2 = 0xb8;
        iVar3 = 0xf;
    }
    *((u8 *)&((UnkStruct_ov14_boxsys *)iVar4)->unk_448[1]) = (*((u8 *)&((UnkStruct_ov14_boxsys *)iVar4)->unk_448[1])) ^ 1;
    ov14_021E7808(param_1, param_2, (s32)(*((s8 *)&((UnkStruct_ov14_boxapp *)param_1)->sys->unk_448[0])), uVar2, iVar3);
    return;
}
