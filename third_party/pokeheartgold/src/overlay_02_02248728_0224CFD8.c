#include "global.h"

#include "field_types_def.h"
#include "player_avatar.h"
#include "field/overlay_01_021FB878.h"

struct Field3dObject;
struct MapObject;
typedef struct MapObject MapObject;
void Field3dObject_SetPosEx(Field3dObject *obj, fx32 x, fx32 y, fx32 z);
MapObject *MapObjectManager_GetFirstActiveObjectByID(const MapObjectManager *mapObjMan, int index);
int MapObject_GetFacingDirection(const MapObject *mapObj);
int MapObject_GetXCoord(const MapObject *mapObj);
int MapObject_GetZCoord(const MapObject *mapObj);
void MapObject_CopyPositionVector(const MapObject *mapObj, VecFx32 *pos);

void ov02_0224CFD8(MapObjectManager *param0, u32 param1, Field3dObject *param2);
void ov02_0224D044(PlayerAvatar *const playerAvatar, Field3dObject *param1);
void ov02_0224D0AC(PlayerAvatar *const playerAvatar, Field3dObject *param1);

void ov02_0224CFD8(MapObjectManager *param0, u32 param1, Field3dObject *param2) {
    int v0;
    int v1, v2;
    VecFx32 v3;
    MapObject *v4 = MapObjectManager_GetFirstActiveObjectByID(param0, param1);

    MapObject_CopyPositionVector(v4, &v3);

    v1 = MapObject_GetXCoord(v4);
    v2 = MapObject_GetZCoord(v4);
    v0 = MapObject_GetFacingDirection(v4);

    switch (v0) {
    case 0:
        v2--;
        break;
    case 1:
        v2++;
        break;
    case 2:
        v1--;
        break;
    case 3:
        v1++;
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    Field3dObject_SetPosEx(param2, (((v1) << 4) * ((fx32)0x0000000000001000L)) + ((16 * ((fx32)0x0000000000001000L)) >> 1), v3.y, (((v2) << 4) * ((fx32)0x0000000000001000L)) + ((16 * ((fx32)0x0000000000001000L)) >> 1));
}

void ov02_0224D044(PlayerAvatar *const playerAvatar, Field3dObject *param1) {
    int v0;
    int v1, v2;
    VecFx32 v3;

    PlayerAvatar_CopyPositionVector(playerAvatar, &v3);

    v1 = PlayerAvatar_GetXCoord(playerAvatar);
    v2 = PlayerAvatar_GetZCoord(playerAvatar);
    v0 = PlayerAvatar_GetFacingDirection(playerAvatar);

    switch (v0) {
    case 0:
        v2--;
        break;
    case 1:
        v2++;
        break;
    case 2:
        v1--;
        break;
    case 3:
        v1++;
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    Field3dObject_SetPosEx(param1, (((v1) << 4) * ((fx32)0x0000000000001000L)) + ((16 * ((fx32)0x0000000000001000L)) >> 1), v3.y, (((v2) << 4) * ((fx32)0x0000000000001000L)) + ((16 * ((fx32)0x0000000000001000L)) >> 1));
}

void ov02_0224D0AC(PlayerAvatar *const playerAvatar, Field3dObject *param1) {
    VecFx32 v0;

    PlayerAvatar_CopyPositionVector(playerAvatar, &v0);
    Field3dObject_SetPosEx(param1, v0.x, v0.y, v0.z);
}
