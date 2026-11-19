#include "global.h"

#include "follow_mon.h"
#include "map_object.h"
#include "overlay_01_022053EC.h"
#include "player_avatar.h"

s32 ov01_021F9048(LocalMapObject *);

void ov01_022059AC(FieldSystem *fieldSystem) {
    LocalMapObject *new_var;
    u32 x;
    u32 y;
    if (fieldSystem->followMon.unk15 != 0) {
        if (fieldSystem->followMon.active == 0) {
            x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
            y = PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
            FollowMon_InitMapObject(fieldSystem->mapObjectManager, (s32)x, (s32)y, (s32)PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar), (u32)fieldSystem->location->mapId);
            new_var = fieldSystem->followMon.mapObject;
            if (fieldSystem->followMon.active == 1) {
                ov01_021F9048(new_var);
            }
        } else if (FollowMon_GetPermissionBySpeciesAndMap(FollowMon_GetSpecies(fieldSystem->followMon.mapObject), (u32)fieldSystem->location->mapId) == 0) {
            MapObject_Remove(fieldSystem->followMon.mapObject);
            fieldSystem->followMon.active = 0;
        }
    }
    fieldSystem->followMon.unk15 = 0;
}

void ov01_02205A34(FieldSystem *fieldSystem, s32 y) {
    VecFx32 positionVector;
    LocalMapObject *object;

    if (FollowMon_IsActive(fieldSystem) != 0) {
        object = FollowMon_GetMapObject(fieldSystem);
        MapObject_CopyPositionVector(object, &positionVector);
        positionVector.y = y;
        MapObject_SetPositionVector(object, &positionVector);
    }
}
