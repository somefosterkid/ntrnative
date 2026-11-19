#include "global.h"

#include "error_handling.h"
#include "map_object.h"
#include "unk_02062108.h"

void MapObject_SetHeldMovement(LocalMapObject *obj, u32 a1) {
    if ((s32)a1 >= 0x71) {
        GF_AssertFail();
    }
    MapObject_SetMovementCommand(obj, a1);
    MapObject_SetMovementStep(obj, 0U);
    MapObject_SetFlagsBits(obj, MAPOBJECTFLAG_UNK4);
    MapObject_ClearFlagsBits(obj, MAPOBJECTFLAG_UNK5);
}

void MapObject_ForceSetHeldMovement(LocalMapObject *arg0, u32 arg1);

void MapObject_ForceSetHeldMovement(LocalMapObject *object, u32 command) {
    MapObject_SetMovementCommand(object, command);
    MapObject_SetMovementStep(object, 0U);
    MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK5);
}
