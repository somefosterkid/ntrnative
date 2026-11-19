#include "global.h"

#include "field_types_def.h"
#include "overlay_01.h"
#include "player_avatar.h"
#include "pokemon_types_def.h"
#include "mon_ride_task.h"
#include "task.h"

void ov01_021F202C(FieldSystem *fieldSystem, int direction, const MonRideTask *monRideTask);

void ov01_021F3040(FieldSystem *fieldSystem, Pokemon *partyMon, MonRideTask *monRideTask);

Pokemon *ov01_021F3100(FieldSystem *, s32);

s32 ov01_022062CC(FieldSystem *);

void CallFieldTask_Surf(TaskManager *taskManager, int playerDirection, int partySlot) {
    int new_var;
    FieldSystem *fieldSystem;
    Pokemon *uVar1;
    s32 sVar2;
    BOOL BVar3;
    s32 iVar4;
    MonRideTask monRideTask;
    fieldSystem = TaskManager_GetFieldSystem(taskManager);
    uVar1 = ov01_021F3100(fieldSystem, partySlot);
    new_var = 0;
    ov01_021F3040(fieldSystem, uVar1, &monRideTask);
    sVar2 = PlayerAvatar_GetState(fieldSystem->playerAvatar);
    if ((sVar2 - 1U) <= (2 - 1)) {
        monRideTask.playCutIn = 1;
    } else {
        BVar3 = ov01_02206268(fieldSystem);
        if ((BVar3 != new_var) && (iVar4 = ov01_022062CC(fieldSystem), partySlot == iVar4)) {
            monRideTask.playCutIn = new_var;
        }
    }
    ov01_021F202C(fieldSystem, playerDirection, &monRideTask);
    return;
}
