#include "global.h"

#include "field_system.h"
#include "overlay_01_021F1AFC.h"
#include "task.h"

WaterfallTaskEnv *ov01_021F28B8(FieldSystem *fieldSystem, int param1, const MonRideTask *monRideTask);

BOOL ov01_021F2944(TaskManager *param0);

void ov01_021F28EC(FieldSystem *fieldSystem, int param1);

void ov01_021F28EC(FieldSystem *fieldSystem, int param1)
{
    WaterfallTaskEnv *taskEnv = ov01_021F28B8(fieldSystem, param1, NULL);
    FieldSystem_CreateTask(fieldSystem, ov01_021F2944, taskEnv);
}
