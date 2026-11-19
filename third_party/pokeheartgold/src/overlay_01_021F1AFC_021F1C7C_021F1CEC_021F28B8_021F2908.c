#include "global.h"

#include "field_system.h"
#include "overlay_01_021F1AFC.h"
#include "pokemon_types_def.h"
#include "task.h"

WaterfallTaskEnv *ov01_021F28B8(FieldSystem *fieldSystem, int param1, const MonRideTask *monRideTask);

BOOL ov01_021F2944(TaskManager *param0);

void ov01_021F3040(FieldSystem *fieldSystem, Pokemon *partyMon, MonRideTask *monRideTask);

Pokemon *ov01_021F3100(FieldSystem *fieldSystem, int partySlot);

void CallFieldTask_Waterfall(TaskManager *task, int direction, int partySlot);

void CallFieldTask_Waterfall(TaskManager *task, int direction, int partySlot)
{
    MonRideTask monRideTask;
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(task);
    Pokemon *partyMon = ov01_021F3100(fieldSystem, partySlot);

    ov01_021F3040(fieldSystem, partyMon, &monRideTask);

    {
        WaterfallTaskEnv *taskEnv = ov01_021F28B8(fieldSystem, direction, &monRideTask);
        TaskManager_Call(task, ov01_021F2944, taskEnv);
    }
}
