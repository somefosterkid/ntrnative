#include "global.h"

#include "overlay_01_map_load_manager.h"
#include "sys_task.h"
#include "map_matrix.h"

struct AreaDataManager;

struct MapLoadSlot;

struct MapLazyLoaderManager;

struct MapLoadQueued;

struct MapLazyLoaderTrackedTarget;

struct MapLoadTasks;

struct MapMatrix;

struct MapPropAnimationManager;

struct MapPropManager;

struct NARC;

struct SysTask;

struct SysTaskManager;

void MapLoadManager_TrackTarget(VecFx32 *position, MapLoadManager *mapLoadManager);

void MapLoadManager_TrackTarget(VecFx32 *position, MapLoadManager *mapLoadManager) {
    mapLoadManager->tracking.pos = *position;
    mapLoadManager->tracking.target = position;
}
