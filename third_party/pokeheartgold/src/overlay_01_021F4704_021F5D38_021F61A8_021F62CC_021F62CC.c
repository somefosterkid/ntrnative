#include "global.h"

#include "field/map_prop_animation.h"

#include "filesystem.h"
#include "map_matrix.h"
#include "sys_task.h"
#include "overlay_01_map_load_manager.h"

struct AreaDataManager;

struct MapPropManager;

BOOL MapLoadManager_HasEnded(const MapLoadManager *mapLoadManager);

BOOL MapLoadManager_HasEnded(const MapLoadManager *mapLoadManager) {
    return mapLoadManager->ended == TRUE && mapLoadManager->numActiveSlots == 0;
}
