#include "global.h"

#include "metatile_behavior.h"

s32 sub_02060ED4(void *arg0, u16 arg1);

struct MapObject;
typedef struct MapObject MapObject;
int MapObject_CheckFlag28(const MapObject *mapObj);
int sub_02060EEC(MapObject *mapObj, u32 tileBehavior);

s32 sub_02060ED4(void *arg0, u16 tile) {
    if (sub_0205B8C4(tile) != 0) {
        return 1;
    }
    return 0;
}

int sub_02060EEC(MapObject *mapObj, u32 tileBehavior) {
    if (MapObject_CheckFlag28(mapObj) == 1 && sub_0205BA30(tileBehavior) == 1) {
        return 1;
    }

    return 0;
}
