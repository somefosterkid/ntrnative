#include "global.h"

struct MapObject;

typedef struct MapObject MapObject;

void MapObject_CopyPositionVector(const MapObject *mapObj, VecFx32 *pos);

void MapObject_CopyFacingVector(const MapObject *mapObj, VecFx32 *vec);

void sub_0205F990(const MapObject *mapObj, VecFx32 *vec);

void sub_0205F9B0(const MapObject *mapObj, VecFx32 *vec);

void ov01_021F93AC(const MapObject *param0, VecFx32 *adjustedObjectPos);

void ov01_021F93AC(const MapObject *param0, VecFx32 *adjustedObjectPos) {
    VecFx32 objectPosition, jumpOffset, posOffset, terrainSpriteOffset;

    MapObject_CopyPositionVector(param0, &objectPosition);
    MapObject_CopyFacingVector(param0, &jumpOffset);
    sub_0205F990(param0, &posOffset);
    sub_0205F9B0(param0, &terrainSpriteOffset);

    adjustedObjectPos->x = objectPosition.x + jumpOffset.x + posOffset.x + terrainSpriteOffset.x;
    adjustedObjectPos->y = objectPosition.y + jumpOffset.y + posOffset.y + terrainSpriteOffset.y;
    adjustedObjectPos->z = objectPosition.z + jumpOffset.z + posOffset.z + terrainSpriteOffset.z;
}
