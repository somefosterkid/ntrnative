#include "global.h"

#include "field_types_def.h"
#include "follow_mon.h"
#include "map_object.h"
#include "task.h"
#include "unk_02062108.h"

s32 ov01_02205A60(TaskManager *taskMan);

s32 ov01_02205A60(TaskManager *taskMan) {
    FieldSystem *temp_r5;
    LocalMapObject *temp_r5_2;
    u32 *temp_r4;
    u32 temp_r0;

    temp_r5 = TaskManager_GetFieldSystem(taskMan);
    temp_r4 = TaskManager_GetStatePtr(taskMan);
    if (FollowMon_IsActive(temp_r5) == 0) {
        return 1;
    }
    temp_r0 = *temp_r4;
    temp_r5_2 = temp_r5->followMon.mapObject;
    switch (temp_r0) {
    case 0:
        if (MapObject_CheckMovementPaused(temp_r5_2) != 0) {
            MapObject_UnpauseMovement(temp_r5_2);
            *temp_r4 = 1;
        } else {
            *temp_r4 = 2;
        }
        break;
    case 1:
        if ((MapObject_IsMovementPaused(temp_r5_2) != 0) && (MapObject_AreBitsSetForMovementScriptInit(temp_r5_2) != 0)) {
            MapObject_PauseMovement(temp_r5_2);
            return 1;
        }
        break;
    case 2:
        if ((MapObject_IsMovementPaused(temp_r5_2) != 0) && (MapObject_AreBitsSetForMovementScriptInit(temp_r5_2) != 0)) {
            return 1;
        }
        break;
    }
    return 0;
}
