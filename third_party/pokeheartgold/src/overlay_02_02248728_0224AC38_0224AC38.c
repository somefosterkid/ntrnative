#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "math_util.h"
#include "overworld_anim_manager.h"
#include "sprite.h"
#include "overlay_02_hmcut_in.h"


struct HMCutIn;


struct Sprite;

struct SysTask;



void ov02_0224AC38(HMCutIn *cutIn);

void ov02_0224AC38(HMCutIn *cutIn) {
    VecFx32 birdPosition = { FX32_ONE * (128 + 8), FX32_ONE * (96 - 8), 0 };
    VecFx32 birdScale = { 0x400, 0x400, 0 };
    FlyTaskEnv *env = sub_02068D74(cutIn->flyBirdAnim);

    env->stage = 1;
    env->state = 0;
    env->subState = 0;
    env->birdStartPos = birdPosition;
    env->pathOffset.x = 0;
    env->pathOffset.y = 0;
    env->pathOffset.z = 0;
    env->birdRotation = FX32_ONE * 60;
    env->birdScale = birdScale;
    env->scaleDelta = 0x200;
    env->angleParam = FX32_ONE * 180;
    env->pathParam = 0;
    env->pathDelta = 0x2000;

    Sprite_SetMatrix(env->birdSprite, &birdPosition);
    Sprite_SetAffineScale(env->birdSprite, &birdScale);
    Sprite_SetAffineZRotation(env->birdSprite, GF_DegreeToSinCosIdx(env->birdRotation / FX32_ONE));
    Sprite_SetDrawFlag(env->birdSprite, TRUE);
}
