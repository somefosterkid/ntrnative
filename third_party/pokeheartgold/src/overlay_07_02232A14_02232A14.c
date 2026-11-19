#include "global.h"

#include "overlay_07_ball_capsule_seal_effect.h"
#include "sys_task_api.h"

s32 ov07_0221C69C(void);

void ov07_02232A5C(SysTask *task, void *data);

void ov07_02232AFC(BallCapsuleSealEffect *sealEffect, void (*callback)(SPLEmitter *));

void ov07_02232A14(BallCapsuleSealEffect *arg0, void (*arg1)(SPLEmitter *));

void ov07_02232A14(BallCapsuleSealEffect *arg0, void (*arg1)(SPLEmitter *)) {
    arg0->isActive = 1;
    ov07_0221C69C();
    ov07_02232AFC(arg0, arg1);
    arg0->task = SysTask_CreateOnMainQueue(ov07_02232A5C, arg0, 0x3E8U);
}
