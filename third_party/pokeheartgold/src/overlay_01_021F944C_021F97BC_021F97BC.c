#include "global.h"
#include "metatile_behavior.h"

struct MapObject;

struct MapObjectManager;

typedef struct MapObject MapObject;

typedef struct MapObjectManager MapObjectManager;

int MapObjectManager_GetObjectCount(const MapObjectManager *mapObjMan);

const MapObject *MapObjectManager_GetObjects2(const MapObjectManager *mapObjMan);

void MapObjectArray_NextObject(const MapObject **mapObj);

u32 MapObject_GetSpriteID(const MapObject *mapObj);

int MapObject_CheckActive(const MapObject *mapObj);


int BerryPatchGraphics_GetCurrentGraphicsResourceID(const MapObject *mapObject);

int ov01_021F97BC(const MapObjectManager *param0, const MapObject *param1, int param2);

int ov01_021F97BC(const MapObjectManager *param0, const MapObject *param1, int param2) {
    int v0, v1;
    const MapObject *v2;

    v0 = MapObjectManager_GetObjectCount(param0);
    v2 = MapObjectManager_GetObjects2(param0);

    do {
        if (v2 != param1) {
            if (MapObject_CheckActive(v2) == 1) {
                v1 = MapObject_GetSpriteID(v2);

                if ((v1 != 0xffff) && (v1 == param2)) {
                    return 1;
                }
            }
        }

        MapObjectArray_NextObject(&v2);
        v0--;
    } while (v0);

    return 0;
}
