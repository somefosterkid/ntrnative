#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "map_object.h"

void sub_020659CC(LocalMapObject *object);

void sub_020659CC(LocalMapObject *object) {
    FieldSystem *temp_r4;
    s32 temp_r6_2;
    u32 temp_r4_2;
    u32 temp_r7;
    u8 *temp_r6;

    temp_r4 = MapObject_GetFieldSystem(object);
    temp_r6 = sub_0205F394(object);
    MapObject_ClearSingleMovement(object);
    MapObject_ClearEndMovement(object);
    MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK5);
    MapObject_SetMovementCommand(object, 0xFFU);
    MapObject_SetMovementStep(object, 0U);
    *temp_r6 = 0;
    temp_r4->followMon.unk4 = 0;
    temp_r4->followMon.unk1C = 0;
    temp_r4->followMon.unk8 = 0;
    temp_r4->followMon.unkC = 0;
    temp_r4_2 = MapObject_GetXCoord(object);
    temp_r6_2 = MapObject_GetYCoord(object);
    temp_r7 = MapObject_GetZCoord(object);
    MapObject_SetPositionFromXYZAndDirection(object, temp_r4_2, (u32)temp_r6_2, temp_r7, MapObject_GetFacingDirection(object));
}
