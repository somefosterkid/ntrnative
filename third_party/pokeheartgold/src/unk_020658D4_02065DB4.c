#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "player_avatar.h"

u32 sub_02065DB4(LocalMapObject *param_1);

u32 sub_02065DB4(LocalMapObject *param_1) {
    FieldSystem *fieldSystem;
    PlayerAvatar *avatar;
    LocalMapObject *object;
    u32 uVar1;

    fieldSystem = MapObject_GetFieldSystem(param_1);
    avatar = FieldSystem_GetPlayerAvatar(fieldSystem);
    object = PlayerAvatar_GetMapObject(avatar);
    uVar1 = MapObject_GetMovementCommand(object);
    switch (uVar1) {
    case 0x58:
        return 0x10;
    case 0x59:
        return 0x11;
    case 0x5a:
        return 0x12;
    case 0x5b:
        uVar1 = 0x13;
    }
    return uVar1;
}
