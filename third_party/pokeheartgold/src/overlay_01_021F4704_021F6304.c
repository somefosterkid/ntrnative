#include "global.h"

#include "field/map_load_manager.h"

#include "overlay_01_map_load_manager.h"

void MapLoadManager_ForgetTrackedTarget(MapLoadManager *mapLoadManager) {
    mapLoadManager->tracking.target = NULL;
}
