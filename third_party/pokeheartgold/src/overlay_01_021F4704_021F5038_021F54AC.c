#include "global.h"

#include "field/area_data.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/overlay_01_02204004.h"

#include "error_handling.h"
#include "filesystem.h"
#include "map_matrix.h"
#include "sys_task.h"
#include "overlay_01_map_load_manager.h"

void ov01_021F474C(MapLoadManager *mapLoadManager, const u8 index);

BOOL ov01_021F5038(MapLoadManager *mapLoadManager, const int firstMapMatrixIndex, const int secondMapMatrixIndex, const u8 firstQuadrantToLoad, const u8 secondQuadrantToLoad, const u8 lazyLoaderManIndex, const u8 trackedTargetDirection);

BOOL ov01_021F5BBC(const u8 trackedTargetDirection, const MapLoadManager *mapLoadManager, u8 *managerIndex);

void ov01_021F54AC(const int firstMapMatrixIndex, const int secondMapMatrixIndex, const u8 firstQuadrantToLoad, const u8 secondQuadrantToLoad, const u8 trackedTargetDirection, MapLoadManager *mapLoadManager);

void ov01_021F54AC(const int firstMapMatrixIndex, const int secondMapMatrixIndex, const u8 firstQuadrantToLoad, const u8 secondQuadrantToLoad, const u8 trackedTargetDirection, MapLoadManager *mapLoadManager) {
    u8 managerIndex;
    BOOL res;

    ((mapLoadManager->queued.queued != 1) ? (void)0 : GF_AssertFail());

    if (ov01_021F5BBC(trackedTargetDirection, mapLoadManager, &managerIndex) == 1) {
        ov01_021F474C(mapLoadManager, managerIndex);

        if (mapLoadManager->numActiveSlots >= 2) {
            mapLoadManager->queued.queued = 1;
            mapLoadManager->queued.matrixIndices[0] = firstMapMatrixIndex;
            mapLoadManager->queued.matrixIndices[1] = secondMapMatrixIndex;
            mapLoadManager->queued.mapSlots[0] = firstQuadrantToLoad;
            mapLoadManager->queued.mapSlots[1] = secondQuadrantToLoad;
            mapLoadManager->queued.maps[0] = mapLoadManager->maps[firstQuadrantToLoad];
            mapLoadManager->queued.maps[1] = mapLoadManager->maps[secondQuadrantToLoad];
            mapLoadManager->queued.direction = trackedTargetDirection;
            mapLoadManager->maps[firstQuadrantToLoad]->matrixIndex = firstMapMatrixIndex;
            mapLoadManager->maps[secondQuadrantToLoad]->matrixIndex = secondMapMatrixIndex;
        } else {
            res = ov01_021F5038(mapLoadManager, firstMapMatrixIndex, secondMapMatrixIndex, firstQuadrantToLoad, secondQuadrantToLoad, mapLoadManager->nextSlot, trackedTargetDirection);
        }
    } else {
        res = ov01_021F5038(mapLoadManager, firstMapMatrixIndex, secondMapMatrixIndex, firstQuadrantToLoad, secondQuadrantToLoad, mapLoadManager->nextSlot, trackedTargetDirection);
        ((res == 1) ? (void)0 : GF_AssertFail());
    }
}
