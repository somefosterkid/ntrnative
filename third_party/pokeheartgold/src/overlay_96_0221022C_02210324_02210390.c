#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "overlay_96_app_0221040c.h"

u32 ov96_02210390(Ov96App_0221040C *param_1, u8 param_2, u16 param_3) {
    u32 uVar1;

    if (param_1 == NULL) {
        GF_AssertFail();
    }
    if (param_2 >= 4) {
        GF_AssertFail();
    }
    ManagedSprite_SetAnim(param_1->sprites[param_2 + 3], param_2 + 4);
    uVar1 = ov96_021E5F24(param_1->course);
    if ((param_2 == uVar1) && (param_3 != param_1->unk_E6)) {
        uVar1 = ov96_02210324(param_1, param_2, param_3);
        if (uVar1 != 1) {
            if (uVar1 == 2) {
                PlaySE_SetPitch(0x88d, 0x80);
            } else if (uVar1 == 3) {
                PlaySE_SetPitch(0x88d, 0x100);
            } else {
                PlaySE_SetPitch(0x88d, 0x140);
            }
        }
        return 1;
    }
    return 0;
}
