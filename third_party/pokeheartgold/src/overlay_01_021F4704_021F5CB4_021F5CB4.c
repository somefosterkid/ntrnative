#include "global.h"

#include "overlay_01_map_load_manager.h"

#include "field/area_data.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/overlay_01_02204004.h"

#include "error_handling.h"
#include "filesystem.h"
#include "map_matrix.h"
#include "sys_task.h"

BOOL ov01_021F5BF0(MapLoadManager *mapLoadManager);

void ov01_021F5CB4(MapLoadManager *mapLoadManager);

void ov01_021F5CB4(MapLoadManager *mapLoadManager) {
    MapLoadQueued *queued = &mapLoadManager->queued;

    if (mapLoadManager->numActiveSlots >= 2) {
        ((0) ? (void)0 : GF_AssertFail());
    }

    BOOL res = ov01_021F5BF0(mapLoadManager);
    ((res == 1) ? (void)0 : GF_AssertFail());

    queued->maps[0]->matrixIndex = queued->matrixIndices[0];
    queued->maps[1]->matrixIndex = queued->matrixIndices[1];

    mapLoadManager->numActiveSlots++;
    mapLoadManager->nextSlot = (mapLoadManager->nextSlot + 1) % 2;
}
