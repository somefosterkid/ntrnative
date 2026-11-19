#include "global.h"
#include "script_manager.h"

struct MapObject;

typedef struct MapObject MapObject;

typedef int (*DistanceFunc)(const MapObject *, int, int, int, int);

extern const DistanceFunc _020FE194[4];

int sub_0206439C(const MapObject *mapObj, int direction, int range, int playerX, int playerZ, int unused);

int MapObject_GetXCoord(const MapObject *mapObj);
int MapObject_GetZCoord(const MapObject *mapObj);
int MapObject_GetYCoord(const MapObject *mapObj);
u32 sub_02060B90(const MapObject *mapObj, int x, int y, int z, int dir);
int GetDeltaXByFacingDirection(int dir);
int GetDeltaYByFacingDirection(int dir);
u32 MapObject_GetScriptID(const MapObject *mapObj);
int sub_020643B8(const MapObject *mapObj, int range, int playerX, int playerZ, int unused);
int sub_020643E4(const MapObject *mapObj, int range, int playerX, int playerZ, int unused);
int sub_02064410(const MapObject *mapObj, int range, int playerX, int playerZ, int unused);
int sub_0206443C(const MapObject *mapObj, int range, int playerX, int playerZ, int unused);
BOOL sub_02064468(const MapObject *mapObj, int direction, int distance);
int MapObject_GetTrainerNum(MapObject *mapObj);

int sub_0206439C(const MapObject *mapObj, int direction, int range, int playerX, int playerZ, int unused) {
    return _020FE194[direction](mapObj, range, playerX, playerZ, unused);
}

int sub_020643B8(const MapObject *mapObj, int range, int playerX, int playerZ, int unused) {
    int mapObjX = MapObject_GetXCoord(mapObj);

    if (mapObjX == playerX) {
        int mapObjZ = MapObject_GetZCoord(mapObj);

        if ((playerZ < mapObjZ) && (playerZ >= (mapObjZ - range))) {
            return mapObjZ - playerZ;
        }
    }

    return -1;
}

int sub_020643E4(const MapObject *mapObj, int range, int playerX, int playerZ, int unused) {
    int mapObjX = MapObject_GetXCoord(mapObj);

    if (mapObjX == playerX) {
        int mapObjZ = MapObject_GetZCoord(mapObj);

        if ((playerZ > mapObjZ) && (playerZ <= (mapObjZ + range))) {
            return playerZ - mapObjZ;
        }
    }

    return -1;
}

int sub_02064410(const MapObject *mapObj, int range, int playerX, int playerZ, int unused) {
    int mapObjZ = MapObject_GetZCoord(mapObj);

    if (mapObjZ == playerZ) {
        int mapObjX = MapObject_GetXCoord(mapObj);

        if ((playerX < mapObjX) && (playerX >= (mapObjX - range))) {
            return mapObjX - playerX;
        }
    }

    return -1;
}

int sub_0206443C(const MapObject *mapObj, int range, int playerX, int playerZ, int unused) {
    int mapObjZ = MapObject_GetZCoord(mapObj);

    if (mapObjZ == playerZ) {
        int mapObjX = MapObject_GetXCoord(mapObj);

        if ((playerX > mapObjX) && (playerX <= (mapObjX + range))) {
            return playerX - mapObjX;
        }
    }

    return -1;
}

BOOL sub_02064468(const MapObject *mapObj, int direction, int distance) {
    u32 collisionFlags;

    if (distance == 0) {
        return 1;
    }

    int targetX = MapObject_GetXCoord(mapObj);
    int targetZ = MapObject_GetZCoord(mapObj);
    int targetY = MapObject_GetYCoord(mapObj);
    targetX += GetDeltaXByFacingDirection(direction);
    targetZ += GetDeltaYByFacingDirection(direction);

    for (int distCovered = 0; distCovered < (distance - 1); distCovered++) {
        collisionFlags = sub_02060B90(mapObj, targetX, targetY, targetZ, direction);
        collisionFlags &= ~(1 << 0);

        if (collisionFlags) {
            return 1;
        }

        targetX += GetDeltaXByFacingDirection(direction);
        targetZ += GetDeltaYByFacingDirection(direction);
    }

    collisionFlags = sub_02060B90(mapObj, targetX, targetY, targetZ, direction);
    collisionFlags &= ~(1 << 0);

    if (collisionFlags == (1 << 2)) {
        return 0;
    }

    return 1;
}

int MapObject_GetTrainerNum(MapObject *mapObj) {
    return ScriptNumToTrainerNum(MapObject_GetScriptID(mapObj));
}
