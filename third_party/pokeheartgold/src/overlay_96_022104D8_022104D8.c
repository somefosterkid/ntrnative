#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"

extern ManagedSpriteTemplate ov96_0221D17C;

ManagedSprite *ov96_022104D8(SpriteSystem *param_1, SpriteManager *param_2, s16 param_3, u8 param_4, u16 param_5, u16 param_6);

ManagedSprite *ov96_022104D8(SpriteSystem *param_1, SpriteManager *param_2, s16 param_3, u8 param_4, u16 param_5, u16 param_6) {
    ManagedSprite *managedSprite;
    ManagedSpriteTemplate MStack_4c;

    MStack_4c = ov96_0221D17C;
    if (param_1 == NULL) {
        GF_AssertFail();
    }
    if (param_2 == NULL) {
        GF_AssertFail();
    }
    MStack_4c.x = param_3;
    MStack_4c.y = (s16)param_4;
    MStack_4c.animation = param_5;
    MStack_4c.drawPriority = (s32)param_6;
    managedSprite = SpriteSystem_NewSpriteWithYOffset(param_1, param_2, &MStack_4c, FX32_CONST(480));
    ManagedSprite_SetAnimateFlag(managedSprite, 1);
    return managedSprite;
}
