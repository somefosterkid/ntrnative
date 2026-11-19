#include "global.h"

#include "field/map_prop_animation.h"

#include "filesystem.h"
#include "map_matrix.h"
#include "sys_task.h"
#include "overlay_01_map_load_manager.h"

struct AreaDataManager;

struct MapPropManager;

void ov01_021F5568(s32 index, MapLoadManager *mapLoadManager);

void ov01_021F55F4(const u8 firstResetLoadedMap, const u8 secondResetLoadedMap, const u8 firstKeptLoadedMap, const u8 secondKeptLoadedMap, MapLoadManager *mapLoadManager);

void ov01_021F55F4(const u8 firstResetLoadedMap, const u8 secondResetLoadedMap, const u8 firstKeptLoadedMap, const u8 secondKeptLoadedMap, MapLoadManager *mapLoadManager) {
    LoadedMap *tmp[2];

    ov01_021F5568(firstResetLoadedMap, mapLoadManager);
    ov01_021F5568(secondResetLoadedMap, mapLoadManager);

    tmp[0] = mapLoadManager->maps[firstResetLoadedMap];
    tmp[1] = mapLoadManager->maps[secondResetLoadedMap];

    mapLoadManager->maps[firstResetLoadedMap] = mapLoadManager->maps[firstKeptLoadedMap];
    mapLoadManager->maps[secondResetLoadedMap] = mapLoadManager->maps[secondKeptLoadedMap];

    mapLoadManager->maps[firstKeptLoadedMap] = tmp[0];
    mapLoadManager->maps[secondKeptLoadedMap] = tmp[1];
}
