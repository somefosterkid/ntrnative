#include "global.h"

#include "sprite.h"
#include "sprite_system.h"

extern u8 _020FFF5C[];

void *sub_020755E8(void *param_1, void *param_2, s32 param_3, s32 param_4, u8 param_5);

void *sub_020755E8(void *param_1, void *param_2, s32 param_3, s32 param_4, u8 param_5) {
    ManagedSprite *managedSprite;
    ManagedSpriteTemplate MStack_48;
    MStack_48 = *((ManagedSpriteTemplate *)((u8 *)_020FFF5C));
    MStack_48.x = (s16)param_3;
    MStack_48.y = (s16)param_4;
    MStack_48.animation = (u16)((u8)param_5);
    managedSprite = SpriteSystem_NewSprite((SpriteSystem *)param_1, (SpriteManager *)param_2, &MStack_48);
    ManagedSprite_SetAnimateFlag(managedSprite, 1);
    return (u8 *)managedSprite;
}
