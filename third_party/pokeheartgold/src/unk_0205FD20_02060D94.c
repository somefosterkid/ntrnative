#include "global.h"

struct MapObject;
typedef struct MapObject MapObject;
int MapObject_GetXRange(const MapObject *mapObj);
int MapObject_GetYRange(const MapObject *mapObj);
int MapObject_GetInitialX(const MapObject *mapObj);
int MapObject_GetInitialZ(const MapObject *mapObj);

int sub_02060D94(const MapObject *mapObj, int x, int y, int z);

int sub_02060D94(const MapObject *mapObj, int x, int y, int z) {
    int initialPos, movementRange, minPos, maxPos;

    initialPos = MapObject_GetInitialX(mapObj);
    movementRange = MapObject_GetXRange(mapObj);

    if (movementRange != -1) {
        minPos = initialPos - movementRange;
        maxPos = initialPos + movementRange;

        if (minPos > x || maxPos < x) {
            return 1;
        }
    }

    initialPos = MapObject_GetInitialZ(mapObj);
    movementRange = MapObject_GetYRange(mapObj);

    if (movementRange != -1) {
        minPos = initialPos - movementRange;
        maxPos = initialPos + movementRange;

        if (minPos > z || maxPos < z) {
            return 1;
        }
    }

    return 0;
}
