#include "global.h"

#include "overlay_07_ball_capsule_seal_effect.h"
#include "sys_task.h"
#include "sys_task_api.h"

ParticleSystem *ov07_0221FEB0(enum HeapID heapID, s32, s32, s32);

s32 ov07_022325F4(s32);

void ov07_02232730(SysTask *task, void *data);

void ov07_022329B0(BallCapsuleSealEffect *sealEffect) {
    s32 v0;

    if (sealEffect->hasCapsule == 0) {
        v0 = ov07_022325F4(sealEffect->ballType);
        sealEffect->sealCount = 1;
        sealEffect->sealParticleSystems[0] = ov07_0221FEB0(sealEffect->heapID, 0x5F, v0, 0);
        sealEffect->state = 0xFF;
        return;
    }
    sealEffect->sealCount = 0;
    sealEffect->state = 0;
    ov07_02232730(SysTask_CreateOnMainQueue(ov07_02232730, sealEffect, 0x3E8U), sealEffect);
}
