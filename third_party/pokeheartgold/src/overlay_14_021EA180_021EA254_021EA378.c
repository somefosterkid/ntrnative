#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021EA378(UnkStruct_ov14_boxapp *param_1);

u32 ov14_021E65C4(UnkStruct_ov14_boxapp *param_1);
void ov14_021E6CF8(UnkStruct_ov14_boxapp *param_1, u32 param_2, u32 param_3);
u32 ov14_021E70B0(UnkStruct_ov14_boxapp *param_1, s32 param_2);
void ov14_021E7148(UnkStruct_ov14_boxapp *param_1, void *param_2);
void ov14_021F29E4(UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);

void sub_02019978(void *, s32);
s32 ov14_021EA378(UnkStruct_ov14_boxapp *param_1) {
    u32 uVar1;
    s32 iVar2;
    u8 *iVar3;
    u32 uVar4;

    iVar3 = ((u8 *)param_1->sys);
    sub_02019978(*((void **)&((UnkStruct_ov14_boxsys *)iVar3)->unk_2F0), 0xf);
    switch (((UnkStruct_ov14_boxsys *)iVar3)->unk_10) {
    default:
        break;
    case 0:
        ov14_021F29E4(param_1->sys, 9, 9);
        uVar4 = (u32) param_1->sys->unk_44C;
        if ((uVar4 >= 0x25) && (uVar4 <= 0x2A)) {
            uVar4 = uVar4 + 0x5b;
        }
        ov14_021E6CF8(param_1, (u32) param_1->unk21, uVar4);
        if ((uVar4 & 0x80) == 0) {
            uVar1 = ov14_021E70B0(param_1, (u32) param_1->unk21);
            *((s8 *)&param_1->unk21) = (s8)uVar1;
        }
        ((UnkStruct_ov14_boxsys *)iVar3)->unk_10 = ((UnkStruct_ov14_boxsys *)iVar3)->unk_10 + 1;
    case 1:
        iVar2 = ov14_021E65C4(param_1);
        if (iVar2 == 0) {
            ov14_021E7148(param_1, ((UnkStruct_ov14_boxsys *)iVar3)->unk_C);
            ov14_021F29E4(param_1->sys, 9, 8);
            ((UnkStruct_ov14_boxsys *)iVar3)->unk_10 = 0;
            return 0;
        }
        break;
    }
    return 1;
}
