#include "global.h"

#include "map_object.h"
#include "player_avatar.h"
#include "unk_02062108.h"

s32 sub_0205DE64(u32 movementAction);

s32 sub_0205CF60(PlayerAvatar *playerAvatar);

s32 sub_0205CF60(PlayerAvatar *playerAvatar) {
    LocalMapObject *localMapObject;
    u32 v0;
    u32 v1;

    v1 = PlayerAvatar_GetMoveState(playerAvatar);
    v0 = PlayerAvatar_GetPlayerMoveState(playerAvatar);
    if (v1 == 0) {
        return 1;
    }
    if (v1 == 2) {
        return 1;
    }
    switch (v1) {
    case 1:
        if ((v0 == 0) || (v0 == 3)) {
            return 1;
        }
        localMapObject = PlayerAvatar_GetMapObject(playerAvatar);
        if (MapObject_AreBitsSetForMovementScriptInit(localMapObject) == 1) {
            return 1;
        }
        if (sub_0205DE64(MapObject_GetMovementCommand(localMapObject)) == 1) {
            return 1;
        }
    default:
        return 0;
    }
}
