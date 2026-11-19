#include "global.h"

struct MapObject;

typedef struct MapObject MapObject;

void MapObject_SetFlagsBits(MapObject *mapObj, u32 flag);

void *sub_0205F3E4(MapObject *mapObj);

void MapObject_IncrementMovementStep(MapObject *mapObj);

void MapObject_SetFacingVector(MapObject *mapObj, const VecFx32 *vec);

void sub_02066438(MapObject *param0);

typedef struct RevealTrainerMovementData {
    u32 jumpHeightIndex;
} RevealTrainerMovementData;

extern const fx32 *_0210FACC[];

enum {
    JUMP_HEIGHT_HIGH,
    JUMP_HEIGHT_LOW,
    JUMP_HEIGHT_NORMAL,
};

BOOL MapObjectMovementCmd101_Step1(MapObject *mapObj);

BOOL MapObjectMovementCmd101_Step1(MapObject *mapObj) {
    RevealTrainerMovementData *data = sub_0205F3E4(mapObj);
    const fx32 *jumpHeightsTable = _0210FACC[JUMP_HEIGHT_HIGH];
    VecFx32 v2 = { 0, 0, 0 };

    v2.y = jumpHeightsTable[data->jumpHeightIndex];
    MapObject_SetFacingVector(mapObj, &v2);

    data->jumpHeightIndex += 2;

    if (data->jumpHeightIndex < 16) {
        return 0;
    }

    v2.y = 0;

    MapObject_SetFacingVector(mapObj, &v2);
    MapObject_SetFlagsBits(mapObj, (1 << 3) | (1 << 17) | (1 << 5));
    sub_02066438(mapObj);
    MapObject_IncrementMovementStep(mapObj);

    return 1;
}
