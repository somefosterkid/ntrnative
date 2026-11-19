#include "global.h"

#include "unk_0201956C.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021E783C(UnkStruct_ov14_boxapp *param_1, u32 param_2, s32 param_3);
u32 ov14_021E7930(UnkStruct_ov14_boxapp *param_1, u32 param_2);
void ov14_021E85D0(UnkStruct_0201956C *param_1);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
void ov14_021F2DE8(UnkStruct_ov14_boxapp *param_1, u32 param_2);
void ov14_021F4848(UnkStruct_ov14_boxapp *param_1);
s32 ov14_021E92AC(UnkStruct_ov14_boxapp *arg0);
s32 ov14_021E9370(UnkStruct_ov14_boxapp *arg0);
void ov14_021F0F0C(UnkStruct_ov14_boxapp *param_1, u32 param_2);

void ov14_021F0F0C(UnkStruct_ov14_boxapp *param_1, u32 param_2) {
    u8 bVar1;
    BOOL bVar2;
    u32 uVar3;
    u32 uVar5;
    UnkFuncPtr_ov14_boxtask pcVar6;
    bVar1 = param_1->unk1F;
    param_1->unk1F = param_1->unk25;
    uVar5 = param_1->unk25;
    if (bVar1 > uVar5) {
        ov14_021F2DE8(param_1, param_1->unk1F);
        uVar3 = ov14_021E7930(param_1, (u32)(param_1->unk1F));
        uVar5 = 1;
        ov14_021E783C(param_1, uVar3, 0);
        pcVar6 = ov14_021E92AC;
    } else {
        ov14_021F2DE8(param_1, param_1->unk1F);
        uVar3 = ov14_021E7930(param_1, (u32)(param_1->unk1F));
        uVar5 = 0;
        ov14_021E783C(param_1, uVar3, 1);
        pcVar6 = ov14_021E9370;
    }
    switch (param_2) {
    case 0:
        ov14_021F4848(param_1);
        if (param_1->unk23[0] == 0) {
            param_1->unk30 = 0xc;
        } else {
            param_1->unk30 = 0x24;
        }
        break;

    case 1:
        ov14_021F4848(param_1);
        param_1->unk30 = 0x3d;
        break;

    case 2:
        param_1->unk30 = 0x47;
        break;

    case 3:
        ov14_021F4848(param_1);
        bVar2 = ov14_021E85E4(param_1->sys->unk_2F0);
        if (bVar2 == 1) {
            ov14_021E85D0(param_1->sys->unk_2F0);
        }
        param_1->unk30 = 0x29;
        break;

    case 4:
        ov14_021F4848(param_1);
        bVar2 = ov14_021E85E4(param_1->sys->unk_2F0);
        if (bVar2 == 1) {
            ov14_021E85D0(param_1->sys->unk_2F0);
        }
        param_1->unk30 = 0x82;
    }

    ov14_021F0234(param_1, pcVar6, param_1->unk30);
    return;
}
