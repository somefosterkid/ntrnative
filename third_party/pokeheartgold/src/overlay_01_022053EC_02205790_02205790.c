#include "global.h"

#include "follow_mon.h"
#include "map_object.h"
#include "overlay_01.h"
#include "player_avatar.h"

void ov01_02205790(FieldSystem *fieldSystem, u8 a1) {
    VecFx32 positionVector;
    LocalMapObject *object;

    if (FollowMon_IsActive(fieldSystem) != 0) {
        object = FollowMon_GetMapObject(fieldSystem);
        MapObject_CopyPositionVector(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), &positionVector);
        MapObject_SetPositionFromVectorAndDirection(object, &positionVector, (u32)a1);
    }
}
