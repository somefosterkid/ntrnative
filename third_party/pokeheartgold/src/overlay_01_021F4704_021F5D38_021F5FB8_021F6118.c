#include "global.h"

#include "field/map_load_manager.h"
#include "overlay_01_map_load_manager.h"

void ov01_021F5F64(s32 arg0, s32 arg1, MapLoadManager *arg2);

void MapLoadManager_InitialLoad(MapLoadManager *manager, int playerTileX, int playerTileZ) {
    manager->funcs->initialLoad(manager, playerTileX, playerTileZ, manager->stride);
    ov01_021F5F64(playerTileX, playerTileZ, manager);
}
