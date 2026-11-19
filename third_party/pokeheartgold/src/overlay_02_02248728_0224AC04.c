#include "global.h"

#include "overworld_anim_manager.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_02_hmcut_in.h"

struct HMCutIn;



typedef int (*FlyAwayTaskFunc)(FlyTaskEnv *);

extern const FlyAwayTaskFunc *const ov02_022533C0[];

void ov02_0224AC04(OverworldAnimManager *param0, void *taskEnv);

void ov02_0224AC24(void);

int ov02_0224AC28(FlyTaskEnv *flyTaskEnv);

void ov02_0224AC04(OverworldAnimManager *param0, void *taskEnv) {
    FlyTaskEnv *flyTaskEnv = taskEnv;
    const FlyAwayTaskFunc *taskFuncs;
    taskFuncs = ov02_022533C0[flyTaskEnv->stage];

    while (taskFuncs[flyTaskEnv->state](flyTaskEnv) == 1) {}
}

void ov02_0224AC24(void) {
}

int ov02_0224AC28(FlyTaskEnv *flyTaskEnv) {
    flyTaskEnv->subState = 0;
    Sprite_SetDrawFlag(flyTaskEnv->birdSprite, 0);
    return 0;
}
