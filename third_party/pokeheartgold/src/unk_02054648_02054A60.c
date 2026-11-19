#include "global.h"

#include "field/map_prop.h"

typedef struct TerrainCollisionHitbox {
    fx32 startX;
    fx32 startZ;
    fx32 endX;
    fx32 endZ;
} TerrainCollisionHitbox;

void sub_02054A60(const int tileX, const int tileZ, const int offsetTileX, const int offsetTileZ, const u32 sizeX, const u32 sizeZ, TerrainCollisionHitbox *hitbox);
BOOL sub_02054A9C(const MapProp *mapProp, const TerrainCollisionHitbox *hitbox, const VecFx32 *mapAbsoluteOrigin);

void sub_02054A60(const int tileX, const int tileZ, const int offsetTileX, const int offsetTileZ, const u32 sizeX, const u32 sizeZ, TerrainCollisionHitbox *hitbox) {
    int startTileX = tileX + offsetTileX;
    int startTileZ = tileZ + offsetTileZ;

    int endTileX = startTileX + sizeX;
    int endTileZ = startTileZ + sizeZ;

    if (startTileX < 0 || startTileZ < 0 || endTileX < 0 || endTileZ < 0) {
        ((0) ? (void)0 : GF_AssertFail());
    }

    hitbox->startX = startTileX * (16 * ((fx32)0x0000000000001000L));
    hitbox->startZ = startTileZ * (16 * ((fx32)0x0000000000001000L));
    hitbox->endX = endTileX * (16 * ((fx32)0x0000000000001000L));
    hitbox->endZ = endTileZ * (16 * ((fx32)0x0000000000001000L));
}

BOOL sub_02054A9C(const MapProp *mapProp, const TerrainCollisionHitbox *hitbox, const VecFx32 *mapAbsoluteOrigin) {
    VecFx32 mapPropPosition;
    MapProp_GetTranslation(&mapPropPosition, mapProp);

    mapPropPosition.x += mapAbsoluteOrigin->x;
    mapPropPosition.z += mapAbsoluteOrigin->z;

    if (hitbox->startX <= mapPropPosition.x && mapPropPosition.x <= hitbox->endX && hitbox->startZ <= mapPropPosition.z && mapPropPosition.z <= hitbox->endZ) {
        return 1;
    }

    return 0;
}
