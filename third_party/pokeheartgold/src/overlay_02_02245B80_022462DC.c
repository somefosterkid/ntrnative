#include "global.h"

#include "overworld_anim_manager.h"
#include "sprite.h"
#include "overlay_02_02245B80.h"
#include "sys_task.h"
#include "overlay_02_hmcut_in.h"

void ov02_02246048(void *arg0);

void ov02_022462DC(SysTask *task, void *windParticleAnimEnv);

void ov02_022462DC(SysTask *task, void *windParticleAnimEnv) {
    LegendWingTask *env = windParticleAnimEnv;
    ov02_02246048(env->effect);
}
