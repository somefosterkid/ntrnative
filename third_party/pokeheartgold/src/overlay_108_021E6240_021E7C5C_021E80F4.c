#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_108_safari_area_customizer.h"

u32 ov108_021E80F4(SafariAreaCustomizer *param_1);

u32 ov108_021E80F4(SafariAreaCustomizer *param_1) {
    s32 iVar1;
    u32 dy;
    if (param_1->unk_184E2_3 == 0) {
        param_1->unk_184E4 = param_1->unk_184E4 + 0x18;
        param_1->unk_184E6 = param_1->unk_184E6 + 0x18;
        dy = -0x18;
    } else {
        dy = 0x18;
        param_1->unk_184E4 = param_1->unk_184E4 + (-0x18);
        param_1->unk_184E6 = param_1->unk_184E6 + (-0x18);
    }
    iVar1 = 0;
    param_1->unk_184E2_0 = 1;
    do {
        ManagedSprite_OffsetPositionXY(param_1->sprites[iVar1 + param_1->unk_184E2_2 * 6], 0, dy);
        iVar1 = iVar1 + 1;
    } while (iVar1 < 6);
    Sprite_OffsetPositionXY(param_1->unk_354[0], 0, dy);
    Sprite_OffsetPositionXY(param_1->unk_354[2], 0, dy);
    param_1->unk_184E1 = param_1->unk_184E1 + 1;
    if (param_1->unk_184E1 >= 8) {
        return 1;
    }
    return 0;
}
