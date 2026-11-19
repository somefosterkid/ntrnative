#include "global.h"

#include "overlay_07_ball_capsule_seal_effect.h"
#include "error_handling.h"
#include "sys_task_api.h"
#include "unk_02014DA0.h"

SysTask *ov07_0221FF2C(ParticleSystem *);
void ov07_02232A5C(SysTask *arg0, void *param);

BOOL ov07_02232A54(BallCapsuleSealEffect *sealEffect) {
    return sealEffect->isActive;
}

void ov07_02232A5C(SysTask *arg0, void *param) {
    BallCapsuleSealEffect *arg1 = param;
    SysTask *task;
    ParticleSystem *emitter;
    s32 v0;
    s32 v2;

    task = arg0;
    v2 = 0;
    if (arg1->sealCount > 9) {
        GF_AssertFail();
    }
    for (v0 = 0; v0 < arg1->sealCount; v0++) {
        emitter = arg1->sealParticleSystems[v0];
        if (emitter != NULL) {
            if ((sub_020154B0(emitter) == 0) && (arg1->sealActiveFlags[v0] != 0)) {
                ov07_0221FF2C(arg1->sealParticleSystems[v0]);
                arg1->sealParticleSystems[v0] = NULL;
                arg1->sealActiveFlags[v0] = 0;
            } else {
                v2 = 1;
            }
        }
    }
    if (v2 == 0) {
        arg1->isActive = 0;
        SysTask_Destroy(task);
    }
}
