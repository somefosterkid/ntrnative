#include "global.h"

#include "overworld_anim_manager.h"
#include "sprite.h"
#include "sys_task.h"
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

void ov02_0224B2CC(OverworldAnimManager *param0);

typedef struct HMCutInWithPlayer1 {
    HMCutIn *cutIn;
    Sprite *playerSprite;
} HMCutInWithPlayer1;
int ov02_0224B350(OverworldAnimManager *param0, void *playerAnimEnv);

void ov02_0224B2CC(OverworldAnimManager *param0) {
    VecFx32 playerSpritePosPtr;
    const VecFx32 *playerSpritePos;
    PlayerAnimEnv *env = sub_02068D74(param0);

    env->state = 1;
    env->animationPartFinished = 0;
    env->positionDeltaIdx = 0;
    env->_ = 0;
    env->position.x = 0;
    env->position.y = 0;
    env->position.z = 0;
    playerSpritePos = Sprite_GetMatrixPtr(env->playerSprite);
    playerSpritePosPtr = *playerSpritePos;

    sub_02068DA8(param0, &playerSpritePosPtr);
    Sprite_SetAnimCtrlSeq(env->playerSprite, 5);
}

void ov02_02248C98(Sprite *sprite, VecFx32 *vectorToSet);

void ov02_0224B314(OverworldAnimManager *param0);

void ov02_0224B314(OverworldAnimManager *param0) {
    VecFx32 spritePosition;
    PlayerAnimEnv *env = sub_02068D74(param0);

    env->state = 2;
    env->animationPartFinished = 0;
    env->positionDeltaIdx = 0;
    env->_ = 0;
    env->position.x = 0;
    env->position.y = 0;
    env->position.z = 0;

    ov02_02248C98(env->playerSprite, &spritePosition);

    sub_02068DA8(param0, &spritePosition);
    Sprite_SetAnimCtrlSeq(env->playerSprite, 4);
}

int ov02_0224B350(OverworldAnimManager *param0, void *playerAnimEnv) {
    PlayerAnimEnv *env = playerAnimEnv;
    const HMCutInWithPlayer1 *data = sub_02068D98(param0);

    env->cutIn = data->cutIn;
    env->playerSprite = data->playerSprite;

    return 1;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_02_02248728_0224AC38_022533FC.s (same label), with the context size from the type, not the DS literal.
void ov02_0224B3FC(void *arg0, s32 *arg1);
extern const OverworldAnimManagerFuncs ov02_022534A4;
const OverworldAnimManagerFuncs ov02_022534A4 = OVERWORLD_ANIM_MANAGER_FUNCS(PlayerAnimEnv, ov02_0224B350, sub_02068DD4, ov02_0224B3FC, sub_02068DD0);
#endif
