#include "global.h"

#include "field_system.h"
#include "overlay_01_021F1AFC.h"
#include "pokemon_types_def.h"
#include "task.h"

void ov01_021F2D68(FieldSystem *fieldSystem, int direction, const MonRideTask *monRideTask);

void ov01_021F3040(FieldSystem *fieldSystem, Pokemon *partyMon, MonRideTask *monRideTask);

Pokemon *ov01_021F3100(FieldSystem *fieldSystem, int partySlot);

void CallFieldTask_Whirlpool(TaskManager *task, int direction, int partySlot);

void CallFieldTask_Whirlpool(TaskManager *task, int direction, int partySlot)
{
    MonRideTask monRideTask;
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(task);
    Pokemon *partyMon = ov01_021F3100(fieldSystem, partySlot);

    ov01_021F3040(fieldSystem, partyMon, &monRideTask);
    ov01_021F2D68(fieldSystem, direction, &monRideTask);
}
