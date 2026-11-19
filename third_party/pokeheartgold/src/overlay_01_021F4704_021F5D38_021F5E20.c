#include "global.h"

#include "overlay_01_021F4704.h"
#include "overlay_01_021FB878.h"
#include "overlay_01_map_load_manager.h"

void ov01_021F5E20(MapLoadManager *mapLoadManager, int x, int z, u32 stride) {
    int matrixIndices[4];
    u8 i;

    ov01_021F497C(mapLoadManager);
    ov01_021F47A0(x, z, mapLoadManager->width, mapLoadManager->height, stride, matrixIndices);
    i = 0;
    do {
        mapLoadManager->maps[i]->bdhc = NULL;
        ov01_021F4F0C(matrixIndices[i], i, mapLoadManager->areaDataManager, mapLoadManager->mapMatrix, mapLoadManager->width, mapLoadManager->height, ov01_021FBA00(mapLoadManager->areaDataManager), mapLoadManager);
        i = (u8)(i + 1);
    } while (i < 4);
}
