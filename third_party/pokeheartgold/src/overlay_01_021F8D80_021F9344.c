#include "global.h"

struct MapObject;
typedef struct MapObject MapObject;
u32 MapObject_GetFlagsBitsMask(const MapObject *mapObj, u32 flag);
int MapObject_CheckMovementPaused(const MapObject *mapObj);
int MapObject_CheckFlag4(const MapObject *mapObj);

int ov01_021F9344(const MapObject *param0);

int ov01_021F9344(const MapObject *param0) {
    if (MapObject_CheckMovementPaused(param0) == 1) {
        if (MapObject_CheckFlag4(param0) == 0) {
            return 1;
        }
    }

    if (MapObject_GetFlagsBitsMask(param0, (1 << 8))) {
        return 1;
    }

    return 0;
}
