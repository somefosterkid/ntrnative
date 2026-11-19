#include "global.h"

#include "field/dynamic_terrain_height.h"

#include "heap.h"

BOOL ov01_021FB368(const int tileX, const int tileZ, const DynamicTerrainHeightPlate *plate);

void ov01_021FB3E4(const u8 index, const int startTileX, const int startTileZ, const int sizeX, const int sizeZ, const fx32 height, DynamicTerrainHeightManager *dynamicTerrainHeightMan);

BOOL ov01_021FB368(const int tileX, const int tileZ, const DynamicTerrainHeightPlate *plate) {
    if (!plate->valid) {
        return 0;
    }

    int startTileX = plate->startTileX;
    int startTileZ = plate->startTileZ;
    int endTileX = startTileX + plate->sizeX - 1;
    int endTileZ = startTileZ + plate->sizeZ - 1;

    return startTileX <= tileX && tileX <= endTileX
        && startTileZ <= tileZ && tileZ <= endTileZ;
}

DynamicTerrainHeightManager *DynamicTerrainHeightManager_New(const u8 platesCount, enum HeapID heapID) {
    DynamicTerrainHeightManager *dynamicTerrainHeightMan = Heap_Alloc(heapID, sizeof(DynamicTerrainHeightManager));

    dynamicTerrainHeightMan->plates = Heap_Alloc(heapID, sizeof(DynamicTerrainHeightPlate) * platesCount);
    dynamicTerrainHeightMan->platesCount = platesCount;

    for (u8 i = 0; i < platesCount; i++) {
        dynamicTerrainHeightMan->plates[i].valid = 0;
    }

    return dynamicTerrainHeightMan;
}

void ov01_021FB3E4(const u8 index, const int startTileX, const int startTileZ, const int sizeX, const int sizeZ, const fx32 height, DynamicTerrainHeightManager *dynamicTerrainHeightMan) {
    dynamicTerrainHeightMan->plates[index].startTileX = startTileX;
    dynamicTerrainHeightMan->plates[index].startTileZ = startTileZ;
    dynamicTerrainHeightMan->plates[index].sizeX = sizeX;
    dynamicTerrainHeightMan->plates[index].sizeZ = sizeZ;
    dynamicTerrainHeightMan->plates[index].height = height;
    dynamicTerrainHeightMan->plates[index].valid = 1;
}
