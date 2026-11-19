#include "global.h"

#include "constants/player_avatar.h"

#include "field_types_def.h"

struct MapObject;
typedef struct MapObject MapObject;
MapObject *FollowMon_GetMapObject(PlayerAvatar *playerAvatar);
BOOL MapObject_CheckVisible(const MapObject *mapObj);
BOOL sub_020659B8(const MapObject *mapObj);

BOOL ov01_022057C4(PlayerAvatar *playerAvatar);
BOOL ov01_022057D0(PlayerAvatar *playerAvatar);

BOOL ov01_022057C4(PlayerAvatar *playerAvatar) {
    MapObject *mapObj = FollowMon_GetMapObject(playerAvatar);
    return MapObject_CheckVisible(mapObj);
}

BOOL ov01_022057D0(PlayerAvatar *playerAvatar) {
    MapObject *mapObj = FollowMon_GetMapObject(playerAvatar);
    return sub_020659B8(mapObj);
}
