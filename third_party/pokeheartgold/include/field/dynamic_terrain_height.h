#ifndef POKEHEARTGOLD_FIELD_DYNAMIC_TERRAIN_HEIGHT_H
#define POKEHEARTGOLD_FIELD_DYNAMIC_TERRAIN_HEIGHT_H

#include "global.h"

#include "constants/heap.h"

typedef struct DynamicTerrainHeightPlate {
    int startTileX;
    int startTileZ;
    int sizeX;
    int sizeZ;
    fx32 height;
    BOOL valid;
} DynamicTerrainHeightPlate;

typedef struct DynamicTerrainHeightManager {
    int platesCount;
    DynamicTerrainHeightPlate *plates;
} DynamicTerrainHeightManager;

DynamicTerrainHeightManager *DynamicTerrainHeightManager_New(const u8 platesCount, enum HeapID heapID);
void DynamicTerrainHeightManager_Free(DynamicTerrainHeightManager *dynamicTerrainHeightManager);

#endif // POKEHEARTGOLD_FIELD_DYNAMIC_TERRAIN_HEIGHT_H
