#include "global.h"

#include "unk_020689C8.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_02_hmcut_in.h"

Sprite *ov02_0224A468(HMCutIn *, const VecFx32 *, s32, s32);

s32 ov02_0224ABCC(void *arg0, void *arg1);

struct HMCutIn;
void ov02_0224ABF8(OverworldAnimManager *param0, void *taskEnv);

s32 ov02_0224ABCC(void *a0, void *arg1) {
    FlyTaskEnv *env = arg1;
    VecFx32 a1;

    env->cutIn = *(HMCutIn *const *)sub_02068D98(a0);
    sub_02068DB8(a0, &a1);
    env->birdSprite = ov02_0224A468(env->cutIn, &a1, 0, 0);
    return 1;
}

void ov02_0224ABF8(OverworldAnimManager *param0, void *taskEnv) {
    FlyTaskEnv *flyTaskEnv = taskEnv;
    Sprite_Delete(flyTaskEnv->birdSprite);
}

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_02_02248728_0224AC38_022533FC.s (same label), with the context size from the type, not the DS literal.
void ov02_0224AC04(OverworldAnimManager *param0, void *taskEnv);
void ov02_0224AC24(void);
extern const OverworldAnimManagerFuncs ov02_0225347C;
const OverworldAnimManagerFuncs ov02_0225347C = OVERWORLD_ANIM_MANAGER_FUNCS(FlyTaskEnv, ov02_0224ABCC, ov02_0224ABF8, ov02_0224AC04, ov02_0224AC24);
#endif
