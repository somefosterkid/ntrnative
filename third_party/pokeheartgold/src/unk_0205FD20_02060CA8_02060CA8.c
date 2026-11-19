#include "global.h"

#include "map_object.h"
#include "overlay_01.h"

s32 ov01_02205664(LocalMapObject *, u32 *, u32 *);

s32 sub_02060CA8(LocalMapObject *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 sub_02060CA8(LocalMapObject *object, s32 arg1, s32 arg2, s32 arg3) {
    u32 v2;
    u32 v1;
    LocalMapObject *v0;
    LocalMapObject *localMapObject;
    MapObjectManager *manager;
    s32 v5;
    s32 v6;
    s32 v7;
    u32 v3;
    u32 v4;
    u32 v8;

    localMapObject = object;
    manager = MapObject_GetManager(object);
    v0 = MapObjectManager_GetObjects2(manager);
    v8 = MapObjectManager_GetObjectCount(manager);
loop_1:
    if ((v0 != localMapObject) && (MapObject_GetFlagsBitsMask(v0, MAPOBJECTFLAG_ACTIVE) != 0)) {
        v2 = MapObject_GetXCoord(v0);
        v3 = MapObject_GetZCoord(v0);
        v1 = v3;
        if ((v2 == arg1) && (v3 == arg3)) {
            v5 = MapObject_GetYCoord(v0) - arg2;
            if (v5 < 0) {
                v5 = 0 - v5;
            }
            if (v5 < 2) {
                return 1;
            }
            goto block_9;
        }
block_9:
        v2 = MapObject_GetPreviousXCoord(v0);
        v4 = MapObject_GetPreviousZCoord(v0);
        v1 = v4;
        if ((v2 == arg1) && (v4 == arg3)) {
            v6 = MapObject_GetYCoord(v0) - arg2;
            if (v6 < 0) {
                v6 = 0 - v6;
            }
            if (v6 < 2) {
                return 1;
            }
            goto block_15;
        }
block_15:
        if ((MapObject_GetID(v0) == 0xFD) && (ov01_022055DC(v0) != 0) && (MapObject_CheckVisible(v0) == 0)) {
            ov01_02205664(v0, &v2, &v1);
            if ((v2 == arg1) && (v1 == arg3)) {
                v7 = MapObject_GetYCoord(v0) - arg2;
                if (v7 < 0) {
                    v7 = 0 - v7;
                }
                if (v7 < 2) {
                    return 1;
                }
                goto block_24;
            }
        }
        goto block_24;
    }
block_24:
    MapObjectArray_NextObject(&v0);
    v8 -= 1;
    if (v8 == 0) {
        return 0;
    }
    goto loop_1;
}
