#include "global.h"

#include "field_system.h"
#include "follow_mon.h"
#include "overlay_01.h"
#include "overlay_01_021F1AFC.h"
#include "task.h"

void ov01_021F30F4(RockClimbTaskEnv *taskEnv);

void ov01_0220609C(FieldSystem *, u32);

extern void (*ov01_022069D0[])(void);

extern void (*ov01_022069F0[])(void);

s32 ov01_021F2628(TaskManager *param_1);

s32 ov01_021F2628(TaskManager *param_1) {
    RockClimbTaskEnv *taskEnv;
    LocalMapObject *mapObject;
    s32 result;
    taskEnv = TaskManager_GetEnvironment(param_1);
    do {
        if (taskEnv->monRideTask.playCutIn == 1) {
            result = ((s32(*)(RockClimbTaskEnv *, TaskManager *))ov01_022069D0[taskEnv->state])(taskEnv, param_1);
        } else {
            result = ((s32(*)(RockClimbTaskEnv *, TaskManager *))ov01_022069F0[taskEnv->state])(taskEnv, param_1);
        }
    } while (result == 2);
    if (result == 1) {
        ov01_02205790(taskEnv->fieldSystem, (u8)taskEnv->direction);
        mapObject = FollowMon_GetMapObject(taskEnv->fieldSystem);
        sub_02069DC8(mapObject, 1);
        ov01_0220609C(taskEnv->fieldSystem, 1);
        ov01_021F30F4(taskEnv);
    }
    return result;
}
