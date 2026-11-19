#include "global.h"
#include "field/area_data.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/model_attributes.h"
#include "field/overlay_01_02204004.h"
#include "filesystem.h"
#include "map_matrix.h"
#include "sys_task.h"
#include "overlay_01_021F4704.h"
#include "overlay_01_map_load_manager.h"

void MapLoadManager_RenderLoadedMap(int index, const MapLoadManager *mapLoadManager, ModelAttributes *const modelAttrs);
void MapLoadManager_RenderLoadedMaps(const MapLoadManager *mapLoadManager, ModelAttributes *const modelAttrs);
void ov01_021F61DC(fx32 objectPositionX, fx32 objectPositionZ, int *tileX, int *tileZ);

void ov01_021F613C(u8 direction, MapLoadManager *mapLoadManager) {
    s32 v0;
    s32 v1;
    s32 v2;

    v0 = ov01_021F5A80(direction, mapLoadManager->height, mapLoadManager->stride, mapLoadManager->currentTile);
    v2 = ov01_021F5A04(v0, mapLoadManager->width, mapLoadManager->stride);
    v1 = ov01_021F6328(v0, mapLoadManager->stride);
    if ((mapLoadManager->currentMatrixIndex == v2) && (mapLoadManager->quadrant != v1)) {
        ov01_021F562C(v2, direction, mapLoadManager);
        ov01_021F5B24(direction, mapLoadManager->quadrant, &mapLoadManager->currentMapSlot);
    }
}

void MapLoadManager_RenderLoadedMaps(const MapLoadManager *mapLoadManager, ModelAttributes *const modelAttrs) {
    for (u8 i = 0; i < 4; i++) {
        if (mapLoadManager->maps[i]->matrixIndex != -1) {
            MapLoadManager_RenderLoadedMap(i, mapLoadManager, modelAttrs);
        }
    }
}

void ov01_021F61DC(fx32 objectPositionX, fx32 objectPositionZ, int *tileX, int *tileZ) {
    *tileX = objectPositionX / (16 * ((fx32)0x0000000000001000L));
    *tileZ = objectPositionZ / (16 * ((fx32)0x0000000000001000L));
}
