#include "global.h"
#include "field_types_def.h"
#include "overlay_02_fly_landing.h"
#include "sys_task.h"
#include "sys_task_api.h"

s32 ov02_0224B364(void *, s32 *);

s32 ov02_0224B3B0();

void ov02_0224B3FC(void *arg0, s32 *arg1);

void ov02_0224B45C(SysTask *task, void *data);
void *ov02_0224B690(s32, s32);
SysTask *ov02_0224B418(FieldSystem *fieldSystem, s32 gender);

s32 ov02_0224B43C(SysTask *unk);

void ov02_0224B3FC(void *arg0, s32 *arg1) {
    s32 v0;

    v0 = *arg1;
    switch (v0) {
    case 1:
        ov02_0224B364(arg0, arg1);
        return;
    case 2:
        ov02_0224B3B0();
        return;
    }
}

SysTask *ov02_0224B418(FieldSystem *fieldSystem, s32 gender) {
    FlyLandingEnv *v0;

    v0 = ov02_0224B690(4, sizeof(FlyLandingEnv));
    v0->playerGender = gender;
    v0->fieldSystem = fieldSystem;
    return SysTask_CreateOnMainQueue(ov02_0224B45C, v0, 0x86U);
}

s32 ov02_0224B43C(SysTask *unk) {
    FlyLandingEnv *env = SysTask_GetData(unk);

    return env->animationFinished;
}
