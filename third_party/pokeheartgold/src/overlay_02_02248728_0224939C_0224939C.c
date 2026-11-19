#include "global.h"

#include "field_system_def.h"
#include "overlay_01_021FCD2C.h"
#include "sprite.h"
#include "overlay_02_fly_landing.h"

void ov02_02249444(FieldSystem *fieldSystem, BOOL set);

s32 ov02_0224939C(FlyLandingEnvExtended *env);

s32 ov02_0224939C(FlyLandingEnvExtended *env) {
    s32 v0;

    v0 = env->subState + 1;
    env->subState = v0;
    if (v0 == 8) {
        Sprite_SetAnimCtrlSeq(env->resourcesWithPlayer.playerSprite, 1);
        ov02_02249444(env->resourcesWithPlayer.fieldSystem, 0);
    }
    if (env->subState == 0xA) {
        Sprite_SetDrawFlag(env->resourcesWithPlayer.playerSprite, 0);
    }
    if ((env->subState > 0xF) && (ov01_021FCD6C(env->task) == 1)) {
        env->subState = 0;
        env->state = (u8)(env->state + 1);
        env->hasEndedIfTwo = 2;
    }
    return 0;
}
