#include "global.h"

#include "field_system.h"
#include "overlay_01.h"
#include "overlay_01_021F1AFC.h"
#include "task.h"
#include "unk_02005D10.h"

void ov01_021F30F4(WaterfallTaskEnv *taskEnv);
void ov01_021F3054(FieldSystem *fieldSystem, MonRideTask *monRideTask);
BOOL ov01_021F3068(MonRideTask *monRideTask);

extern s32 (*ov01_022069A8[5])();

extern s32 (*ov01_022069BC[5])();

BOOL ov01_021F2944(TaskManager *param0);
s32 ov01_021F2998(WaterfallTaskEnv *taskEnv);
s32 ov01_021F29C0(WaterfallTaskEnv *taskEnv);

BOOL ov01_021F2944(TaskManager *param0) {
    WaterfallTaskEnv *taskEnv;
    s32 iVar2;

    taskEnv = TaskManager_GetEnvironment(param0);
    do {
        if (taskEnv->direction == 0) {
            iVar2 = ov01_022069A8[taskEnv->state](taskEnv);
        } else {
            iVar2 = ov01_022069BC[taskEnv->state](taskEnv);
        }
    } while (iVar2 == 2);
    if (iVar2 == 1) {
        ov01_02205790(taskEnv->fieldSystem, (u8)taskEnv->direction);
        ov01_021F30F4(taskEnv);
        return 1;
    }
    return 0;
}

s32 ov01_021F2998(WaterfallTaskEnv *taskEnv) {
    if (taskEnv->monRideTask.playCutIn == 1) {
        ov01_021F3054(taskEnv->fieldSystem, &taskEnv->monRideTask);
        taskEnv->state = taskEnv->state + 1;
        return 0;
    }
    taskEnv->state = 2;
    return 1;
}

s32 ov01_021F29C0(WaterfallTaskEnv *taskEnv) {
    s32 iVar1;

    iVar1 = ov01_021F3068(&taskEnv->monRideTask);
    if (iVar1 == 1) {
        PlaySE(0x64c);
        taskEnv->state = taskEnv->state + 1;
    }
    return 0;
}
