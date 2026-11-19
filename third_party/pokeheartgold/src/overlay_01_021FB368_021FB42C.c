#include "global.h"

#include "field/dynamic_terrain_height.h"

#include "error_handling.h"

BOOL ov01_021FB368(const int tileX, const int tileZ, const DynamicTerrainHeightPlate *plate);

BOOL ov01_021FB42C(const int tileX, const int tileZ, const DynamicTerrainHeightManager *dynamicTerrainHeightMan, u8 *index);

fx32 ov01_021FB474(const u8 index, const DynamicTerrainHeightManager *dynamicTerrainHeightMan);

BOOL ov01_021FB42C(const int tileX, const int tileZ, const DynamicTerrainHeightManager *dynamicTerrainHeightMan, u8 *index) {
    GF_ASSERT(index != NULL);

    for (u8 i = 0; i < dynamicTerrainHeightMan->platesCount; i++) {
        DynamicTerrainHeightPlate *plate = &dynamicTerrainHeightMan->plates[i];

        if (ov01_021FB368(tileX, tileZ, plate)) {
            *index = i;
            return TRUE;
        }
    }

    return FALSE;
}

fx32 ov01_021FB474(const u8 index, const DynamicTerrainHeightManager *dynamicTerrainHeightMan) {
    ((index < dynamicTerrainHeightMan->platesCount) ? (void)0 : GF_AssertFail());
    ((dynamicTerrainHeightMan->plates[index].valid) ? (void)0 : GF_AssertFail());

    return dynamicTerrainHeightMan->plates[index].height;
}
