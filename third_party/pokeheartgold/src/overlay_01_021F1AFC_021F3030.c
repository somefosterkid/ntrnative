#include "global.h"
struct FieldSystem;

#include "field_types_def.h"
#include "map_object.h"
#include "player_avatar.h"
#include "mon_ride_task.h"
#include "overlay_01_021F1AFC.h"
#include "overlay_02.h"
#include "sys_task.h"

void ov01_021F3030(SysTask *task, PlayerMovementSavingWork *work);

void ov01_021F3040(FieldSystem *fieldSystem, Pokemon *partyMon, MonRideTask *monRideTask);
void ov01_021F3054(FieldSystem *fieldSystem, MonRideTask *monRideTask);

void ov01_021F3030(SysTask *task, PlayerMovementSavingWork *work) {
    LocalMapObject *object;

    object = PlayerAvatar_GetMapObject(work->playerAvatar);
    sub_0205F484(object);
    return;
}

void ov01_021F3040(FieldSystem *fieldSystem, Pokemon *partyMon, MonRideTask *monRideTask) {
    monRideTask->playCutIn = 1;
    monRideTask->partyMon = partyMon;
    monRideTask->playerGender = PlayerAvatar_GetGender(fieldSystem->playerAvatar);
}

void ov01_021F3054(FieldSystem *fieldSystem, MonRideTask *monRideTask) {
    monRideTask->HMCutInTask = ov02_02249458(fieldSystem, 0, monRideTask->partyMon, monRideTask->playerGender);
}
