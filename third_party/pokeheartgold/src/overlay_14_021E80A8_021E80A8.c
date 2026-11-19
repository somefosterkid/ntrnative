#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F32E0(void *param_1);
u32 ov14_021E80A8(u8 *param_1);

s32 sub_02019978(void *, u32);

void sub_02019B1C(void *, u32, s8 *, s8 *);

u32 ov14_021E80A8(u8 *param_1) {
    s32 iVar1;
    s32 iVar2;
    s8 cStack_18[4];

    sub_02019B1C(((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys->unk_2F0), 1, (cStack_18 + 3), &cStack_18[2]);
    iVar1 = sub_02019978(((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys->unk_2F0), 1);
    iVar2 = sub_02019978(((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys->unk_2F0), 10);
    sub_02019978(((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys->unk_2F0), 0xf);
    sub_02019B1C(((u8 *)((UnkStruct_ov14_boxapp *)param_1)->sys->unk_2F0), 1, &cStack_18[1], &cStack_18[0]);
    if (((cStack_18 + 3)[0] != cStack_18[1]) || (cStack_18[2] != cStack_18[0])) {
        ov14_021F32E0(param_1);
    }
    if ((iVar1 == 0) && (iVar2 == 0)) {
        return 0;
    }
    return 1;
}
