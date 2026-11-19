#include "global.h"

#include "sprite.h"
#include "overlay_02_fly_landing.h"

s32 ov02_022491A8(FlyLandingEnvExtended *env);

s32 ov02_022491A8(FlyLandingEnvExtended *env) {
    Sprite_SetDrawFlag(env->birdSprite, 0);
    Sprite_SetAnimCtrlSeq(env->resourcesWithPlayer.playerSprite, 1);
    env->subState = 0;
    env->state = (u8)(env->state + 1);
    return 1;
}
