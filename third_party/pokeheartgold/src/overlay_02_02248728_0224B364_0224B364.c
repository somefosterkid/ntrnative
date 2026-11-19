#include "global.h"

#include "sprite.h"
#include "unk_020689C8.h"
#include "overlay_02_hmcut_in.h"

struct HMCutIn;

typedef struct PlayerAnimEnv {
    int state;
    BOOL animationPartFinished;
    BOOL _;
    int positionDeltaIdx;
    VecFx32 position;
    HMCutIn *cutIn;
    Sprite *playerSprite;
} PlayerAnimEnv;

extern const fx32 ov02_02253520[12];

void ov02_0224B364(OverworldAnimManager *param0, PlayerAnimEnv *env);

void ov02_0224B364(OverworldAnimManager *param0, PlayerAnimEnv *env) {
    VecFx32 newPos;

    if (env->animationPartFinished == FALSE) {
        env->position.y = ov02_02253520[env->positionDeltaIdx];
        sub_02068DB8(param0, &newPos);
        newPos.y += env->position.y;
        Sprite_SetMatrix(env->playerSprite, &newPos);
        env->positionDeltaIdx++;

        if (env->positionDeltaIdx >= 12) {
            env->positionDeltaIdx = 0;
            env->_ = TRUE;
            env->animationPartFinished++; // setting the flag to 1 = TRUE
        }
    }
}

extern const fx32 ov02_02253430[4];

void ov02_0224B3B0(OverworldAnimManager *param0, PlayerAnimEnv *env);

void ov02_0224B3B0(OverworldAnimManager *param0, PlayerAnimEnv *env) {
    VecFx32 newPos;

    if (env->animationPartFinished == FALSE) {
        env->position.y = ov02_02253430[env->positionDeltaIdx];
        sub_02068DB8(param0, &newPos);
        newPos.y += env->position.y;
        Sprite_SetMatrix(env->playerSprite, &newPos);
        env->positionDeltaIdx++;

        if (env->positionDeltaIdx >= 4) {
            env->positionDeltaIdx = 0;
            env->_ = TRUE;
            env->animationPartFinished++; // setting the flag to 1 = TRUE
        }
    }
}
