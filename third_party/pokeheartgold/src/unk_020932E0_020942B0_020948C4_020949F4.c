#include "global.h"

#include "sprite.h"
#include "touchscreen.h"
#include "unk_020932E0.h"


void sub_020949F4(UnkStruct_020932E0 *param_1) {
    u32 uStack_2c;
    VecFx32 VStack_1c;
    u32 uStack_30;
    VecFx32 new_var;
    if ((param_1->unk_4680) != 0) {
        System_GetTouchHeldCoords(&uStack_2c, &uStack_30);
        VStack_1c.x = uStack_2c << 0xc;
        VStack_1c.y = uStack_30 << 0xc;
        VStack_1c.z = 0;
        new_var = VStack_1c;
        new_var.y -= 0x4000;
        Sprite_SetMatrix(param_1->unk_46A0[param_1->unk_4684].unk_0, &new_var);
        Sprite_SetMatrix(param_1->unk_8C0, &VStack_1c);
        param_1->unk_4694 = uStack_2c;
        param_1->unk_4698 = uStack_30;
    }
    return;
}
