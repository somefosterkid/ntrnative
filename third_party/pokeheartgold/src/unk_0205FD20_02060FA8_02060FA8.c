#include "global.h"

#include "map_object.h"
#include "unk_02054648.h"
#include "unk_0205FD20.h"

u32 sub_02060FA8(LocalMapObject *arg0, s32 arg1);

u32 sub_02060FA8(LocalMapObject *object, s32 direction) {
    u32 v0;
    s32 v1;
    s32 v3;
    u32 v2;

    v2 = MapObject_GetXCoord(object);
    v3 = GetDeltaXByFacingDirection(direction);
    v0 = MapObject_GetZCoord(object);
    v1 = GetDeltaYByFacingDirection(direction);
    return GetMetatileBehavior(MapObject_GetFieldSystem(object), v2 + v3, v0 + v1);
}
