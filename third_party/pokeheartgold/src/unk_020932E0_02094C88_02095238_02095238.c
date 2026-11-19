#include "global.h"

#include "sprite.h"
#include "unk_020932E0.h"

u32 _s32_div_f(s32, u32);

void sub_02095238(UnkStruct_020932E0 *param_1) {
    BOOL bVar1;
    u32 uVar2;
    s32 iVar3;
    u32 uVar4;
    u32 remainder;
    VecFx32 VStack_24;
    VecFx32 VStack_30;

    bVar1 = FALSE;
    uVar2 = sub_02095DD8(param_1->unk_46B8);
    iVar3 = sub_02095DE8(param_1->unk_46B8);
    VStack_24.z = 0;
    switch (uVar2) {
    case 5:
        VStack_24.x = 0x50000;
        VStack_24.y = 0x10000;
        bVar1 = TRUE;
        break;
    case 2:
    case 6:
        remainder = iVar3 % 6;
        VStack_24.x = ((remainder & 0xff) * 0x18 + 0x14) * 0x1000;
        uVar4 = iVar3 / 6;
        VStack_24.y = ((uVar4 & 0xff) * 0x18 + 0x28) * 0x1000;
        bVar1 = TRUE;
        break;
    case 3:
        VStack_24.x = 0xd4000;
        uVar4 = (iVar3 + 1) * 0x28;
        VStack_24.y = uVar4 * 0x1000;
        bVar1 = TRUE;
        break;
    case 4:
        VStack_24.x = 0xd4000;
        uVar4 = (iVar3 + 1) * 0x28;
        VStack_24.y = uVar4 * 0x1000;
        if (param_1->unk_46BC != 0) {
            VStack_30 = VStack_24;
            VStack_30.y -= 0x4000;
            Sprite_SetMatrix(param_1->unk_46A0[param_1->unk_4684].unk_0,
                &VStack_30);
            param_1->unk_4694 = 0xd4;
            param_1->unk_4698 = uVar4;
        }
        bVar1 = TRUE;
        break;
    case 1:
        VStack_24.x = 0xe0000;
        VStack_24.y = 0xB0000;
        bVar1 = TRUE;
        break;
    }
    if (bVar1) {
        Sprite_SetMatrix(param_1->unk_8C0, &VStack_24);
        sub_020956B8(param_1);
    }
    return;
}
