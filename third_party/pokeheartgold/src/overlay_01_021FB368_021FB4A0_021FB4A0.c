#include "global.h"

#include "field/dynamic_terrain_height.h"

#include "error_handling.h"

void ov01_021FB4A0(const u8 index, const fx32 height, DynamicTerrainHeightManager *dynamicTerrainHeightMan);

void ov01_021FB4A0(const u8 index, const fx32 height, DynamicTerrainHeightManager *dynamicTerrainHeightMan) {
    GF_ASSERT(index < dynamicTerrainHeightMan->platesCount);
    dynamicTerrainHeightMan->plates[index].height = height;
}
