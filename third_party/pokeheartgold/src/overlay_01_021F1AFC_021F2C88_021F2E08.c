#include "global.h"

#include "overlay_01_021F1AFC.h"

void ov01_021F3054(FieldSystem *fieldSystem, MonRideTask *monRideTask);

s32 ov01_021F2E08(WhirlpoolTaskEnv *taskEnv);

s32 ov01_021F2E08(WhirlpoolTaskEnv *taskEnv) {
    ov01_021F3054(taskEnv->fieldSystem, &taskEnv->monRideTask);
    taskEnv->state = taskEnv->state + 1;
    return 0;
}
