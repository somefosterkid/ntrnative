#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_112_sprite_app.h"

void ov112_021F1D28(Ov112SpriteApp *param_1, s32 param_2);

void ov112_021F1D28(Ov112SpriteApp *param_1, s32 param_2) {
    u16 uVar1;
    BOOL BVar2;
    BVar2 = ManagedSprite_GetDrawFlag(param_1->sprites[param_2]);
    if ((BVar2 != 0) && (BVar2 = ManagedSprite_IsAnimated(param_1->sprites[param_2]), BVar2 == 0)) {
        uVar1 = ManagedSprite_GetActiveAnim(param_1->sprites[param_2]) + 1;
        ManagedSprite_SetAnim(param_1->sprites[param_2], uVar1);
    }
    return;
}
