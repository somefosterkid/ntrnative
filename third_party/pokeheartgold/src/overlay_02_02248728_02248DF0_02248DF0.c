#include "global.h"

#include "overworld_anim_manager.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_02_hmcut_in.h"

struct HMCutIn;



typedef int (*FlyAwayTaskFunc)(FlyTaskEnv *);

extern const FlyAwayTaskFunc *const ov02_02253320[];

void ov02_02248DF0(OverworldAnimManager *param0, void *taskEnv);

void ov02_02248DF0(OverworldAnimManager *param0, void *taskEnv) {
    FlyTaskEnv *flyTaskEnv = taskEnv;
    const FlyAwayTaskFunc *taskFuncs;
    taskFuncs = ov02_02253320[flyTaskEnv->stage];

    while (taskFuncs[flyTaskEnv->state](flyTaskEnv) == 1) {}
}
