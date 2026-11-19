#include "global.h"

#include "overlay_01_021F4704.h"
#include "overlay_01_map_load_manager.h"

#include "field/map_prop_animation.h"

#include "filesystem.h"
#include "map_matrix.h"
#include "sys_task.h"

struct AreaDataManager;

struct MapPropManager;

BOOL ov01_021F5BBC(const u8 trackedTargetDirection, const MapLoadManager *mapLoadManager, u8 *managerIndex);

BOOL ov01_021F5BF0(MapLoadManager *mapLoadManager);

BOOL ov01_021F5BBC(const u8 trackedTargetDirection, const MapLoadManager *mapLoadManager, u8 *managerIndex) {
    for (int i = 0; i < 2; i++) {
        if (mapLoadManager->slots[i].active == 1 && ov01_021F5B88(trackedTargetDirection) == mapLoadManager->slots[i].direction) {
            *managerIndex = i;
            return 1;
        }
    }

    return 0;
}

BOOL ov01_021F5BF0(MapLoadManager *mapLoadManager) {
    MapLoadQueued *queued = &mapLoadManager->queued;

    if (mapLoadManager->slots[mapLoadManager->nextSlot].active != 0) {
        ((0) ? (void)0 : GF_AssertFail());
    }

    mapLoadManager->slots[mapLoadManager->nextSlot].active = 1;
    mapLoadManager->slots[mapLoadManager->nextSlot].direction = queued->direction;

    mapLoadManager->slots[mapLoadManager->nextSlot].matrixIndices[0] = queued->matrixIndices[0];
    mapLoadManager->slots[mapLoadManager->nextSlot].matrixIndices[1] = queued->matrixIndices[1];

    mapLoadManager->slots[mapLoadManager->nextSlot].mapSlots[0] = queued->mapSlots[0];
    mapLoadManager->slots[mapLoadManager->nextSlot].mapSlots[1] = queued->mapSlots[1];

    mapLoadManager->slots[mapLoadManager->nextSlot].maps[0] = queued->maps[0];
    mapLoadManager->slots[mapLoadManager->nextSlot].maps[1] = queued->maps[1];

    mapLoadManager->slots[mapLoadManager->nextSlot].numStarted = 0;
    mapLoadManager->slots[mapLoadManager->nextSlot].finished = 0;

    return 1;
}
