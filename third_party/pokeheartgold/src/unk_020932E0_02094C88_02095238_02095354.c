#include "global.h"

#include "sprite.h"
#include "unk_020932E0.h"

u32 sub_02095354(UnkStruct_020932E0 *param_1) {
    u8 uVar1;
    u8 uVar2;
    s32 iVar3;
    VecFx32 *pVVar4;
    u32 uVar5;

    uVar5 = 0;
    uVar1 = sub_02095DD8(param_1->unk_46B8);
    uVar2 = sub_02095DE8(param_1->unk_46B8);
    switch (uVar1) {
    case 2:
    case 6:
        iVar3 = sub_02094794(param_1, uVar2);
        if (iVar3 == 1) {
            sub_020948C4(param_1, 1, uVar2);
            if ((s32)param_1->unk_D > 0) {
                do {
                    VecFx32 VStack_20 = { 0, 0, 0 };
                    VecFx32 VStack_2c;
                    if (param_1->unk_46A4[uVar5].unk_4 == 0) {
                        param_1->unk_4694 = 0xd4;
                        param_1->unk_4698 = (((u8)uVar5) + 1) * 0x28;
                        VStack_20.x = param_1->unk_4694 << 0xc;
                        VStack_20.y = param_1->unk_4698 * 0x1000;
                        VStack_2c = VStack_20;
                        VStack_2c.y = VStack_2c.y + -0x4000;
                        Sprite_SetMatrix(param_1->unk_46A0[param_1->unk_4684].unk_0, &VStack_2c);
                        Sprite_SetMatrix(param_1->unk_8C0, &VStack_20);
                        sub_02095D40(param_1->unk_46B8, 4, uVar5 & 0xff);
                        break;
                    }
                    uVar5 = uVar5 + 1;
                } while ((s32)uVar5 < (s32)(u32)param_1->unk_D);
            }
            uVar5 = 1;
            param_1->unk_46BC = 1;
        }
        break;
    case 3:
        iVar3 = sub_02094860(param_1, uVar2);
        if (iVar3 == 1) {
            VecFx32 VStack_38;
            sub_020948C4(param_1, 2, uVar2);
            pVVar4 = Sprite_GetMatrixPtr(param_1->unk_46A0[param_1->unk_4684].unk_0);
            VStack_38 = *pVVar4;
            VStack_38.y = VStack_38.y + -0x4000;
            Sprite_SetMatrix(param_1->unk_46A0[param_1->unk_4684].unk_0,
                &VStack_38);
            uVar5 = 1;
            param_1->unk_46BC = 1;
            param_1->unk_14 = 4;
        }
        break;
    case 1:
        sub_0209569C(param_1);
        uVar5 = 3;
        break;
    case 4:
        sub_02094A70(param_1);
    }
    return uVar5;
}
