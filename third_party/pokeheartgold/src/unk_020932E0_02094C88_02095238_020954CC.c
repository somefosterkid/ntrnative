#include "global.h"

#include "sprite.h"
#include "unk_02005D10.h"
#include "unk_020932E0.h"

u32 sub_020954CC(UnkStruct_020932E0 *param_1) {
    s32 iVar1;
    u32 uVar2;
    VecFx32 VStack_18;

    uVar2 = 0;
    iVar1 = sub_02095DD8(param_1->unk_46B8);
    switch (iVar1) {
    case 4:
        PlaySE(0x5dc);
        sub_02094A90(param_1, 0);
        break;
    case 1:
        sub_0209569C(param_1);
        uVar2 = 3;
        break;
    default:
        if (param_1->unk_13 != 0) {
            sub_02095D40(param_1->unk_46B8, 1, 0);
            VStack_18.x = 0xe0000;
            VStack_18.y = 0xb0000;
            VStack_18.z = 0;
            Sprite_SetMatrix(param_1->unk_8C0, &VStack_18);
            PlaySE(0x5dc);
        }
        break;
    }
    return uVar2;
}
