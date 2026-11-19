#include "global.h"

#include "sprite.h"
#include "unk_02005D10.h"
#include "unk_020932E0.h"

void _s32_div_f(u32, u32);

void sub_02095540(UnkStruct_020932E0 *param_1) {
    s32 iVar2;
    u32 uVar3;
    u32 remainder;
    VecFx32 VStack_18;
    s32 cVar1;
    if (param_1->unk_F == 0) {
        cVar1 = param_1->unk_10 - 1;
    } else {
        cVar1 = param_1->unk_F - 1;
    }
    param_1->unk_F = cVar1;
    Sprite_SetAnimCtrlSeq(param_1->unk_8BC, 1);
    sub_020943EC(param_1);
    PlaySE(0x5dc);
    if (param_1->unk_F == 0x12) {
        iVar2 = sub_02095DD8(param_1->unk_46B8);
        if (iVar2 == 2) {
            uVar3 = sub_02095DE8(param_1->unk_46B8);
            remainder = (s32)(uVar3) % (s32)(6);
            sub_02095D40(param_1->unk_46B8, 6, remainder & 0xff);
            VStack_18.x = ((remainder & 0xff) * 0x18 + 0x14) * 0x1000;
            VStack_18.y = 0x28000;
            VStack_18.z = 0;
            Sprite_SetMatrix(param_1->unk_8C0, &VStack_18);
            return;
        }
    } else {
        iVar2 = sub_02095DD8(param_1->unk_46B8);
        if (iVar2 == 6) {
            uVar3 = sub_02095DE8(param_1->unk_46B8);
            sub_02095D40(param_1->unk_46B8, 2, uVar3);
        }
    }
    return;
}

void sub_020955EC(UnkStruct_020932E0 *param_1) {
    s32 iVar2;
    u32 uVar3;
    u32 remainder;
    VecFx32 VStack_18;
    param_1->unk_F++;
    if (param_1->unk_F == param_1->unk_10) {
        param_1->unk_F = 0;
    }
    Sprite_SetAnimCtrlSeq(param_1->unk_8B8, 3);
    sub_020943EC(param_1);
    PlaySE(0x5dc);
    if (param_1->unk_F == 0x12) {
        iVar2 = sub_02095DD8(param_1->unk_46B8);
        if (iVar2 == 2) {
            uVar3 = sub_02095DE8(param_1->unk_46B8);
            remainder = (s32)(uVar3) % (s32)(6);
            sub_02095D40(param_1->unk_46B8, 6, remainder & 0xff);
            VStack_18.x = ((remainder & 0xff) * 0x18 + 0x14) * 0x1000;
            VStack_18.y = 0x28000;
            VStack_18.z = 0;
            Sprite_SetMatrix(param_1->unk_8C0, &VStack_18);
            return;
        }
    } else {
        iVar2 = sub_02095DD8(param_1->unk_46B8);
        if (iVar2 == 6) {
            uVar3 = sub_02095DE8(param_1->unk_46B8);
            sub_02095D40(param_1->unk_46B8, 2, uVar3);
        }
    }
    return;
}
