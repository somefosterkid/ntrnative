#include "global.h"

#include "field/area_data.h"

#include "map_matrix.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/overlay_01_02204004.h"
#include "error_handling.h"
#include "filesystem.h"
#include "sys_task.h"
#include "overlay_01_021F4704.h"

BOOL ov01_021F5024(const MapLoadTasks *tasksState);

BOOL ov01_021F5038(MapLoadManager *mapLoadManager, const int firstMapMatrixIndex, const int secondMapMatrixIndex, const u8 firstQuadrantToLoad, const u8 secondQuadrantToLoad, const u8 lazyLoaderManIndex, const u8 trackedTargetDirection);

void ov01_021F4FB0(int matrixIndex, int mapSlot, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, s32 arg6, MapLoadManager *mapLoadManager) {
    u32 modelNo;
    u32 header[4];

    modelNo = ov01_021F4A50(header, mapLoadManager, mapMatrix, mapLoadManager->maps[mapSlot], matrixIndex, width * height, 1);
    if (modelNo != 0xffff) {
        ov01_021F4AAC(mapLoadManager, mapLoadManager->maps[mapSlot]);
        ov01_021F4AE4(mapLoadManager, mapLoadManager->maps[mapSlot], header[0]);
        ov01_021F4B1C(mapLoadManager, mapLoadManager->maps[mapSlot], header, matrixIndex);
        ov01_021F4C6C(mapLoadManager, mapLoadManager->maps[mapSlot], header, areaDataManager, modelNo, matrixIndex, arg6, 1);
    }
    return;
}

BOOL ov01_021F5024(const MapLoadTasks *tasksState) {
    return tasksState->modelLoading == 0 && tasksState->bdhcLoading == 0;
}

BOOL ov01_021F5038(MapLoadManager *mapLoadManager, const int firstMapMatrixIndex, const int secondMapMatrixIndex, const u8 firstQuadrantToLoad, const u8 secondQuadrantToLoad, const u8 lazyLoaderManIndex, const u8 trackedTargetDirection) {
    ((mapLoadManager->slots[lazyLoaderManIndex].active == 0) ? (void)0 : GF_AssertFail());

    mapLoadManager->slots[lazyLoaderManIndex].active = 1;
    mapLoadManager->slots[lazyLoaderManIndex].direction = trackedTargetDirection;
    mapLoadManager->slots[lazyLoaderManIndex].matrixIndices[0] = firstMapMatrixIndex;
    mapLoadManager->slots[lazyLoaderManIndex].matrixIndices[1] = secondMapMatrixIndex;
    mapLoadManager->slots[lazyLoaderManIndex].mapSlots[0] = firstQuadrantToLoad;
    mapLoadManager->slots[lazyLoaderManIndex].mapSlots[1] = secondQuadrantToLoad;
    mapLoadManager->slots[lazyLoaderManIndex].maps[0] = mapLoadManager->maps[firstQuadrantToLoad];
    mapLoadManager->slots[lazyLoaderManIndex].maps[1] = mapLoadManager->maps[secondQuadrantToLoad];
    mapLoadManager->slots[lazyLoaderManIndex].numStarted = 0;
    mapLoadManager->slots[lazyLoaderManIndex].finished = 0;

    if (mapLoadManager->numActiveSlots == 0) {
        mapLoadManager->state = MAP_LOAD_STATE_START_CHUNK;
    }

    mapLoadManager->maps[firstQuadrantToLoad]->matrixIndex = firstMapMatrixIndex;
    mapLoadManager->maps[secondQuadrantToLoad]->matrixIndex = secondMapMatrixIndex;
    mapLoadManager->numActiveSlots++;
    mapLoadManager->nextSlot = (mapLoadManager->nextSlot + 1) % 2;

    return 1;
}
