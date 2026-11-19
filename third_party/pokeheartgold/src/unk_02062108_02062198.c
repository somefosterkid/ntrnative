#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "heap.h"
#include "map_object.h"
#include "move_anim_data.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02062108.h"

void MovementScriptMachine(SysTask *task, void *data);


BOOL MapObject_IsMovementPaused(LocalMapObject *mapObj) {
    if (!MapObject_TestFlagsBits(mapObj, MAPOBJECTFLAG_UNK4)) {
        return 1;
    }

    if (!MapObject_TestFlagsBits(mapObj, MAPOBJECTFLAG_UNK5)) {
        return 0;
    }

    return 1;
}

BOOL MapObject_ClearHeldMovementIfActive(LocalMapObject *mapObj) {
    if (!MapObject_TestFlagsBits(mapObj, MAPOBJECTFLAG_UNK4)) {
        return 1;
    }

    if (!MapObject_TestFlagsBits(mapObj, MAPOBJECTFLAG_UNK5)) {
        return 0;
    }

    MapObject_ClearFlagsBits(mapObj, (MapObjectFlagBits)(MAPOBJECTFLAG_UNK4 | MAPOBJECTFLAG_UNK5));

    return 1;
}

void MapObject_ClearHeldMovement(LocalMapObject *mapObj) {
    MapObject_ClearFlagsBits(mapObj, MAPOBJECTFLAG_UNK4);
    MapObject_SetFlagsBits(mapObj, MAPOBJECTFLAG_UNK5);
    MapObject_SetMovementCommand(mapObj, MOVEMENT_NONE);
    MapObject_SetMovementStep(mapObj, 0);
}

EventObjectMovementMan *EventObjectMovementMan_Create(LocalMapObject *mapObj, const MovementScriptCommand *animCmd) {
    MoveAnimData *data = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(MoveAnimData));
    ((data != 0L) ? (void)0 : GF_AssertFail());
    memset(data, 0, sizeof(MoveAnimData));

    int priority = MapObjectManager_GetPriority(MapObject_GetManager(mapObj)) - 1;
    SysTask *task = SysTask_CreateOnMainQueue(MovementScriptMachine, data, priority);
    ((task != 0L) ? (void)0 : GF_AssertFail());

    data->mapObj = mapObj;
    data->animCmd = animCmd;

    return (EventObjectMovementMan *)task;
}
