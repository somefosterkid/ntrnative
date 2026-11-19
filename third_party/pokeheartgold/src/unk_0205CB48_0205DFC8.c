#include "global.h"

#include "constants/player_avatar.h"

#include "field_types_def.h"

struct MapObject;
typedef struct MapObject MapObject;
MapObject *PlayerAvatar_GetMapObject(PlayerAvatar *playerAvatar);
BOOL MapObject_AreBitsSetForMovementScriptInit(const MapObject *mapObj);

BOOL sub_0205DFC8(PlayerAvatar *playerAvatar);

BOOL sub_0205DFC8(PlayerAvatar *playerAvatar) {
    MapObject *mapObj = PlayerAvatar_GetMapObject(playerAvatar);
    return MapObject_AreBitsSetForMovementScriptInit(mapObj);
}
