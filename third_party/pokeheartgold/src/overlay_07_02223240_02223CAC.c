#include "global.h"

#include "sprite_system.h"
#include "overlay_07_swagger_sprite_context.h"

s32 ov07_02222508(void *, s32, s32, s32, s32);

void ov07_02223CAC(void *arg0, s16 arg1, s16 arg2);

void ov07_02223CAC(void *arg0, s16 x, s16 y) {
    ManagedSprite_SetDrawFlag(((SwaggerSpriteContext *)arg0)->sprite, 1);
    ManagedSprite_SetPositionXY(((SwaggerSpriteContext *)arg0)->sprite, x, y);
    *((s32 *)&((SwaggerSpriteContext *)arg0)->scaleStep) = 0;
    ov07_02222508(arg0 + 0x18, 0xA, 0xA, 0xE, 4);
}
