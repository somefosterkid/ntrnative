#include "global.h"

#include "constants/player_avatar.h"

#include "field_types_def.h"

struct MapObject;
typedef struct MapObject MapObject;
MapObject *PlayerAvatar_GetMapObject(PlayerAvatar *playerAvatar);
u32 sub_02060FA8(MapObject *mapObj, int dir);
int PlayerAvatar_GetXCoord(PlayerAvatar *const playerAvatar);
int PlayerAvatar_GetZCoord(PlayerAvatar *const playerAvatar);
int GetDeltaXByFacingDirection(int dir);
int GetDeltaYByFacingDirection(int dir);

u32 sub_0205DFEC(PlayerAvatar *playerAvatar, int dir);
void sub_0205DFFC(PlayerAvatar *playerAvatar, int dir, int *x, int *z);

u32 sub_0205DFEC(PlayerAvatar *playerAvatar, int dir) {
    MapObject *mapObj = PlayerAvatar_GetMapObject(playerAvatar);
    return sub_02060FA8(mapObj, dir);
}

void sub_0205DFFC(PlayerAvatar *playerAvatar, int dir, int *x, int *z) {
    *x = PlayerAvatar_GetXCoord(playerAvatar) + GetDeltaXByFacingDirection(dir);
    *z = PlayerAvatar_GetZCoord(playerAvatar) + GetDeltaYByFacingDirection(dir);
}
