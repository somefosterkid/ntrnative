#include "global.h"

#include "constants/player_avatar.h"

#include "field_types_def.h"
#include "sys_task.h"
#include "mon_ride_task.h"

struct MapObject;
struct Pokemon;
int ov02_0224953C(SysTask *cutInTask);
void ov02_02249548(SysTask *cutInTask);
typedef struct MapObject MapObject;
MapObject *PlayerAvatar_GetMapObject(PlayerAvatar *playerAvatar);
u32 ov01_021FA930(MapObject *mapObj, int dir);
int PlayerAvatar_GetXCoord(PlayerAvatar *const playerAvatar);
int PlayerAvatar_GetZCoord(PlayerAvatar *const playerAvatar);
const MapObjectManager *MapObject_GetManager(const MapObject *mapObj);
MapObject *MapObjectManager_GetFirstObjectWithXAndZ(const MapObjectManager *mapObjMan, int x, int z, int param3);
int GetDeltaXByFacingDirection(int dir);
int GetDeltaYByFacingDirection(int dir);

BOOL ov01_021F3068(MonRideTask *monRideTask);
u32 ov01_021F3084(PlayerAvatar *playerAvatar, int dir);
MapObject *ov01_021F3094(PlayerAvatar *playerAvatar, int param1);

BOOL ov01_021F3068(MonRideTask *monRideTask) {
    if (ov02_0224953C(monRideTask->HMCutInTask) == 1) {
        ov02_02249548(monRideTask->HMCutInTask);
        return 1;
    }

    return 0;
}

u32 ov01_021F3084(PlayerAvatar *playerAvatar, int dir) {
    MapObject *mapObj = PlayerAvatar_GetMapObject(playerAvatar);
    return ov01_021FA930(mapObj, dir);
}

MapObject *ov01_021F3094(PlayerAvatar *playerAvatar, int param1) {
    int v0 = PlayerAvatar_GetXCoord(playerAvatar) + GetDeltaXByFacingDirection(param1);
    int v1 = PlayerAvatar_GetZCoord(playerAvatar) + GetDeltaYByFacingDirection(param1);
    const MapObjectManager *v2 = MapObject_GetManager(PlayerAvatar_GetMapObject(playerAvatar));
    MapObject *v3 = MapObjectManager_GetFirstObjectWithXAndZ(v2, v0, v1, 0);

    return v3;
}
