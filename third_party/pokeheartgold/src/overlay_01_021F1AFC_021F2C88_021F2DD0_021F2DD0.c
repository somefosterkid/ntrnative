#include "global.h"

#include "field_system.h"
#include "overlay_01.h"
#include "overlay_01_021F1AFC.h"
#include "task.h"

void ov01_021F30F4(WhirlpoolTaskEnv *taskEnv);

extern s32 (*ov01_02206994[5])();

BOOL ov01_021F2DD0(TaskManager *task);

BOOL ov01_021F2DD0(TaskManager *task) {
    s32 iVar2;
    WhirlpoolTaskEnv *taskEnv;
    taskEnv = TaskManager_GetEnvironment(task);
    do {
        iVar2 = ov01_02206994[taskEnv->state](taskEnv);
    } while (iVar2 == 2);
    if (iVar2 == 1) {
        ov01_02205790(taskEnv->fieldSystem, (u8)taskEnv->direction);
        ov01_021F30F4(taskEnv);
    }
    return iVar2;
}
