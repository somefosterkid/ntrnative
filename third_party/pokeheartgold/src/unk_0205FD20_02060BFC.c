#include "global.h"

#include "field_types_def.h"

struct MapObject;
typedef struct MapObject MapObject;
int MapObjectManager_GetObjectCount(const MapObjectManager *mapObjMan);
const MapObject *MapObjectManager_GetObjects2(const MapObjectManager *mapObjMan);
void MapObjectArray_NextObject(const MapObject **mapObj);
u32 MapObject_GetFlagsBitsMask(const MapObject *mapObj, u32 flag);
const MapObjectManager *MapObject_GetManager(const MapObject *mapObj);
int MapObject_GetPreviousXCoord(const MapObject *mapObj);
int MapObject_GetPreviousZCoord(const MapObject *mapObj);
int MapObject_GetXCoord(const MapObject *mapObj);
int MapObject_GetYCoord(const MapObject *mapObj);
int MapObject_GetZCoord(const MapObject *mapObj);

int sub_02060BFC(const MapObject *mapObj, int x, int y, int z);

int sub_02060BFC(const MapObject *mapObj, int x, int y, int z) {
    int maxObjects, objX, objZ;
    const MapObjectManager *mapObjMan = MapObject_GetManager(mapObj);
    const MapObject *v4 = MapObjectManager_GetObjects2(mapObjMan);

    maxObjects = MapObjectManager_GetObjectCount(mapObjMan);

    do {
        if (v4 != mapObj
            && MapObject_GetFlagsBitsMask(v4, (1 << 0))
            && !MapObject_GetFlagsBitsMask(v4, (1 << 18))) {
            objX = MapObject_GetXCoord(v4);
            objZ = MapObject_GetZCoord(v4);

            if (objX == x && objZ == z) {
                int objY = MapObject_GetYCoord(v4);
                int v6 = objY - y;

                if (v6 < 0) {
                    v6 = -v6;
                }

                if (v6 < (1 * 2)) {
                    return 1;
                }
            }

            objX = MapObject_GetPreviousXCoord(v4);
            objZ = MapObject_GetPreviousZCoord(v4);

            if (objX == x && objZ == z) {
                int objY = MapObject_GetYCoord(v4);
                int v8 = objY - y;

                if (v8 < 0) {
                    v8 = -v8;
                }

                if (v8 < (1 * 2)) {
                    return 1;
                }
            }
        }

        MapObjectArray_NextObject(&v4);
        maxObjects--;
    } while (maxObjects);

    return 0;
}
