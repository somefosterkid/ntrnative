#include "global.h"

#include "map_object.h"
#include "player_avatar.h"
#include "unk_02062108.h"

s32 sub_0205DA34(PlayerAvatar *, LocalMapObject *, s32);

s32 sub_0205DE64(u32 movementAction);

s32 sub_0205CBEC(PlayerAvatar *arg0, s32 arg1);

s32 sub_0205CBEC(PlayerAvatar *avatar, s32 arg1) {
    LocalMapObject *localMapObject;
    s32 v0;

    localMapObject = PlayerAvatar_GetMapObject(avatar);
    if (MapObject_AreBitsSetForMovementScriptInit(localMapObject) == 1) {
        return 1;
    }
    if (arg1 == -1) {
        return 0;
    }
    if (sub_0205DE64(MapObject_GetMovementCommand(localMapObject)) == 1) {
        v0 = sub_0205DA34(avatar, localMapObject, arg1);
        if (v0 == 0) {
            return 1;
        }
        if ((v0 == 0x20) && (PlayerAvatar_GetState(avatar) == 2)) {
            return 1;
        }
        return 0;
    }
    return 0;
}
