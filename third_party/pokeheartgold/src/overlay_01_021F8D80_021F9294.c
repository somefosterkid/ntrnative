#include "global.h"

#include "constants/player_avatar.h"

struct MapObject;
struct MapObjectManager;
struct PlayerAvatar;
typedef struct MapObject MapObject;
typedef struct PlayerAvatar PlayerAvatar;
MapObject *MapObjectManager_GetMapModelNarc(PlayerAvatar *playerAvatar);
BOOL NARC_Delete(const MapObject *mapObj);
typedef struct MapObjectManager MapObjectManager;
u32 MapObjectManager_GetFlagsBitsMask(const MapObjectManager *mapObjMan, u32 flag);
u32 MapObject_GetFlagsBitsMask(const MapObject *mapObj, u32 flag);
const MapObjectManager *MapObject_GetManager(const MapObject *mapObj);
void sub_0205F484(MapObject *mapObj);
int MapObject_CheckMovementPaused(const MapObject *mapObj);
int MapObject_CheckFlag4(const MapObject *mapObj);

BOOL FldObjSys_CloseMModelNarc(PlayerAvatar *playerAvatar);
void ov01_021F92A0(MapObject *param0);

BOOL FldObjSys_CloseMModelNarc(PlayerAvatar *playerAvatar) {
    MapObject *mapObj = MapObjectManager_GetMapModelNarc(playerAvatar);
    return NARC_Delete(mapObj);
}

void ov01_021F92A0(MapObject *param0) {
    const MapObjectManager *v0 = MapObject_GetManager(param0);

    if (MapObjectManager_GetFlagsBitsMask(v0, (1 << 2))) {
        return;
    }

    if (!MapObject_GetFlagsBitsMask(param0, (1 << 14))) {
        return;
    }

    if ((MapObject_CheckMovementPaused(param0) == 0) || MapObject_CheckFlag4(param0)) {
        sub_0205F484(param0);
    }
}
