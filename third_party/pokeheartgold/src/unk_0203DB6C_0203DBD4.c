#include "global.h"

#include "field_types_def.h"

struct MapObject;
typedef struct MapObject MapObject;
MapObject *PlayerAvatar_GetMapObject(PlayerAvatar *playerAvatar);
int MapObject_GetPositionVectorYCoordUInt(const MapObject *mapObj);

u8 sub_0203DBD4(PlayerAvatar *playerAvatar, MapObject *param1);

u8 sub_0203DBD4(PlayerAvatar *playerAvatar, MapObject *param1) {
    MapObject *v0 = PlayerAvatar_GetMapObject(playerAvatar);

    if (MapObject_GetPositionVectorYCoordUInt(v0) == MapObject_GetPositionVectorYCoordUInt(param1)) {
        return 1;
    }

    return 0;
}
