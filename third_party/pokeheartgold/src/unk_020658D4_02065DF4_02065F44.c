#include "global.h"

#include "error_handling.h"
#include "map_object.h"
#include "player_avatar.h"

s32 MapObject_ForceSetHeldMovement(LocalMapObject *, s32);

s32 sub_02061200(u32, u32, u32, u32);

s32 sub_0206234C(s32, s32);

s32 sub_02065DB4(LocalMapObject *);

s32 sub_02065F44(LocalMapObject *arg0);

s32 sub_02065F44(LocalMapObject *arg0) {
    PlayerAvatar *avatar;
    u32 x, z, px, pz;
    s32 dir;
    s32 r;
    avatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(arg0));
    x = MapObject_GetXCoord(arg0);
    z = MapObject_GetZCoord(arg0);
    px = PlayerAvatar_GetPreviousXCoord(avatar);
    pz = PlayerAvatar_GetPreviousZCoord(avatar);
    if (x != px || z != pz) {
        dir = sub_02065DB4(arg0);
        r = sub_02061200(x, z, px, pz);
        if (dir == 0xFF) {
            GF_AssertFail();
            return 0;
        }
        MapObject_ForceSetHeldMovement(arg0, sub_0206234C(r, dir));
        return 1;
    }
    return 0;
}
