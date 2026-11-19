#include "global.h"

#include "sprite.h"
#include "sprite_system.h"

extern ManagedSpriteTemplate ov111_021E6C34;

ManagedSprite *ov111_021E6330(SpriteSystem *param_1, SpriteManager *param_2, s16 param_3, s16 param_4, u8 param_5, u8 param_6);

ManagedSprite *ov111_021E6330(SpriteSystem *param_1, SpriteManager *param_2, s16 param_3, s16 param_4, u8 param_5, u8 param_6) {
    ManagedSprite *managedSprite;
    ManagedSpriteTemplate MStack_48;

    MStack_48 = ov111_021E6C34;
    MStack_48.x = param_3;
    MStack_48.y = param_4;
    MStack_48.drawPriority = (s32)param_6;
    MStack_48.animation = (u16)param_5;
    managedSprite = SpriteSystem_NewSpriteWithYOffset(param_1, param_2, &MStack_48, FX32_CONST(524));
    ManagedSprite_SetAnimateFlag(managedSprite, 1);
    return managedSprite;
}
