#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"

extern const u8 ov96_0221CFEC[];

extern const u8 ov96_0221D020[];

ManagedSprite *ov96_0220ECA4(SpriteSystem *param_1, SpriteManager *param_2, s16 param_3, s16 param_4, u16 param_5, u16 param_6, s32 param_7);

ManagedSprite *ov96_0220ECA4(SpriteSystem *param_1, SpriteManager *param_2, s16 param_3, s16 param_4, u16 param_5, u16 param_6, s32 param_7) {
    ManagedSprite *managedSprite;
    ManagedSpriteTemplate *pMVar4;
    ManagedSpriteTemplate MStack_7c;
    ManagedSpriteTemplate MStack_48;

    MStack_7c = *(ManagedSpriteTemplate *)ov96_0221CFEC;
    MStack_48 = *(ManagedSpriteTemplate *)ov96_0221D020;
    if (param_7 != 0) {
        pMVar4 = &MStack_48;
    } else {
        pMVar4 = &MStack_7c;
    }
    if (param_1 == NULL) {
        GF_AssertFail();
    }
    if (param_2 == NULL) {
        GF_AssertFail();
    }
    pMVar4->x = param_3;
    pMVar4->y = param_4;
    pMVar4->animation = param_5;
    pMVar4->drawPriority = (u32)param_6;
    managedSprite = SpriteSystem_NewSpriteWithYOffset(param_1, param_2, pMVar4, FX32_CONST(480));
    ManagedSprite_SetAffineOverwriteMode(managedSprite, 2);
    ManagedSprite_SetAnimateFlag(managedSprite, 1);
    ManagedSprite_SetDrawFlag(managedSprite, 0);
    return managedSprite;
}
