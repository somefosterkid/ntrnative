#include "global.h"

#include "map_object.h"
#include "unk_0205FD20.h"

s32 sub_02060B90(LocalMapObject *, s32, s32, s32, s32);

void sub_02060BB8(LocalMapObject *arg0, s32 arg1);

void sub_02060BB8(LocalMapObject *object, s32 direction) {
    u32 v1;
    s32 v0;
    s32 v3;
    u32 v2;

    v2 = MapObject_GetXCoord(object);
    v3 = GetDeltaXByFacingDirection(direction);
    v0 = MapObject_GetYCoord(object);
    v1 = MapObject_GetZCoord(object);
    sub_02060B90(object, v2 + v3, v0, v1 + GetDeltaYByFacingDirection(direction), direction);
}
