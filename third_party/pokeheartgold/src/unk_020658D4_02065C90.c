#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "player_avatar.h"

u32 sub_02065C90(LocalMapObject *param_1, u8 *param_2);

u32 sub_02065C90(LocalMapObject *param_1, u8 *param_2) {
    FieldSystem *fieldSystem;
    PlayerAvatar *avatar;
    LocalMapObject *object;
    BOOL BVar1;
    u32 uVar2;

    fieldSystem = MapObject_GetFieldSystem(param_1);
    avatar = FieldSystem_GetPlayerAvatar(fieldSystem);
    object = PlayerAvatar_GetMapObject(avatar);
    BVar1 = MapObject_TestFlagsBits(object, MAPOBJECTFLAG_UNK4);
    if ((BVar1 == 1) && (BVar1 = MapObject_TestFlagsBits(object, MAPOBJECTFLAG_UNK5), BVar1 == 1)) {
        *param_2 = 0;
    }
    uVar2 = PlayerAvatar_GetPlayerMoveState(avatar);
    if (uVar2 == 3) {
        *param_2 = 0;
    }
    return 0;
}
