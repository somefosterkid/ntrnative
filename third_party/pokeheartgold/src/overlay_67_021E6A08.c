#include "global.h"

#include "overlay_67_app.h"
#include "sprite.h"
#include "sprite_system.h"

void ov67_021E6A08(Ov67App *app, s32 idx, s32 anim) {
    ManagedSprite_SetAnimationFrame(app->sprites[idx], 0);
    ManagedSprite_SetAnim(app->sprites[idx], anim);
    return;
}

void ov67_021E6A28(Ov67App *app, s32 idx, s16 x, s16 y) {
    ManagedSprite_SetPositionXY(app->sprites[idx], x, y);
}
