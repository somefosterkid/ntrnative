#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "player_avatar.h"

void MapObject_ForceSetHeldMovement(LocalMapObject *, s32 arg1);

u32 sub_02065DB4(LocalMapObject *param_1);

u32 sub_02065FBC(LocalMapObject *param_1);

u32 sub_02065FBC(LocalMapObject *param_1) {
    FieldSystem *fieldSystem;
    PlayerAvatar *avatar;
    u32 uVar1;

    fieldSystem = MapObject_GetFieldSystem(param_1);
    avatar = FieldSystem_GetPlayerAvatar(fieldSystem);
    MapObject_GetXCoord(param_1);
    MapObject_GetZCoord(param_1);
    PlayerAvatar_GetPreviousXCoord(avatar);
    PlayerAvatar_GetPreviousZCoord(avatar);
    uVar1 = sub_02065DB4(param_1);
    if (uVar1 == 0xff) {
        return 0;
    }
    MapObject_ForceSetHeldMovement(param_1, uVar1);
    return 1;
}
