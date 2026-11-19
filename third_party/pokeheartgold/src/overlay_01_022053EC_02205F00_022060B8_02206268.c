#include "global.h"

#include "field_types_def.h"
#include "follow_mon.h"
#include "map_object.h"
#include "player_avatar.h"

BOOL ov01_02206268(FieldSystem *fieldSystem);

BOOL ov01_02206268(FieldSystem *fieldSystem) {
    BOOL BVar1;
    LocalMapObject *object;
    u32 uVar2[1];
    u32 uVar3;
    u32 uVar4;
    u32 uVar5;
    LocalMapObject *object_00;

    BVar1 = FollowMon_IsVisible(fieldSystem);
    if (BVar1 == 0) {
        return 0;
    }
    object = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    object_00 = fieldSystem->followMon.mapObject;
    uVar2[0] = MapObject_GetXCoord(object);
    uVar3 = MapObject_GetZCoord(object);
    uVar4 = MapObject_GetXCoord(object_00);
    uVar5 = MapObject_GetZCoord(object_00);
    if (uVar2[0] == uVar4) {
        if ((uVar3 + 1 == uVar5) || (uVar3 - 1 == uVar5)) {
            return 1;
        }
    } else if ((uVar3 == uVar5) && (uVar2[0] + 1 == uVar4 || (uVar2[0] - 1 == uVar4))) {
        return 1;
    }
    return 0;
}
