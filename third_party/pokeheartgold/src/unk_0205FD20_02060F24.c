#include "global.h"

struct MapObject;
typedef struct MapObject MapObject;
int GetDeltaXByFacingDirection(int dir);
int GetDeltaYByFacingDirection(int dir);
void MapObject_SetPreviousX(MapObject *mapObj, int x);
void MapObject_SetPreviousY(MapObject *mapObj, int y);
void MapObject_SetPreviousZ(MapObject *mapObj, int z);
int MapObject_GetXCoord(const MapObject *mapObj);
void MapObject_AddCurrentX(MapObject *mapObj, int dx);
int MapObject_GetYCoord(const MapObject *mapObj);
void MapObject_AddCurrentY(MapObject *mapObj, int dy);
int MapObject_GetZCoord(const MapObject *mapObj);
void MapObject_AddCurrentZ(MapObject *mapObj, int dz);

void sub_02060F24(MapObject *mapObj, int dir);
void sub_02060F78(MapObject *mapObj);

void sub_02060F24(MapObject *mapObj, int dir) {
    MapObject_SetPreviousX(mapObj, MapObject_GetXCoord(mapObj));
    MapObject_SetPreviousY(mapObj, MapObject_GetYCoord(mapObj));
    MapObject_SetPreviousZ(mapObj, MapObject_GetZCoord(mapObj));

    MapObject_AddCurrentX(mapObj, GetDeltaXByFacingDirection(dir));
    MapObject_AddCurrentY(mapObj, 0);
    MapObject_AddCurrentZ(mapObj, GetDeltaYByFacingDirection(dir));
}

void sub_02060F78(MapObject *mapObj) {
    MapObject_SetPreviousX(mapObj, MapObject_GetXCoord(mapObj));
    MapObject_SetPreviousY(mapObj, MapObject_GetYCoord(mapObj));
    MapObject_SetPreviousZ(mapObj, MapObject_GetZCoord(mapObj));
}
