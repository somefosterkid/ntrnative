#include "global.h"
#include "heap.h"
#include "field/dynamic_terrain_height.h"

void DynamicTerrainHeightManager_Free(DynamicTerrainHeightManager *dynamicTerrainHeightManager) {
    Heap_Free(dynamicTerrainHeightManager->plates);
    Heap_Free(dynamicTerrainHeightManager);
}
