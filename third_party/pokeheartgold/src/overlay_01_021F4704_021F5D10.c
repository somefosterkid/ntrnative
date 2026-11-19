#include "global.h"

#include "field/map_prop_animation.h"

#include "filesystem.h"
#include "map_matrix.h"
#include "sys_task.h"
#include "overlay_01_map_load_manager.h"

struct AreaDataManager;
struct MapPropManager;
BOOL ov01_021F5D10(const MapLoadManager *mapLoadManager);
void ov01_021F5D20(MapLoadManager *mapLoadManager);

void ov01_021F5D38(u32 arg0, struct MapLoadManager *arg1);

BOOL ov01_021F5D10(const MapLoadManager *mapLoadManager) {
    return mapLoadManager->slots[mapLoadManager->currentSlot].finished;
}

void ov01_021F5D20(MapLoadManager *mapLoadManager) {
    for (u8 i = 0; i < 4; i++) {
        mapLoadManager->mapStale[i] = 0;
    }
}

void ov01_021F5D38(u32 arg0, MapLoadManager *arg1) {
    switch (arg0) {
    case 4:
        *((s32 *)&arg1->mapStale[2]) = 1;
        *((s32 *)&arg1->mapStale[3]) = 1;
        return;
    case 1:
        *((s32 *)&arg1->mapStale[0]) = 1;
        *((s32 *)&arg1->mapStale[2]) = 1;
        return;
    case 2:
        *((s32 *)&arg1->mapStale[0]) = 1;
        *((s32 *)&arg1->mapStale[1]) = 1;
        return;
    case 3:
        *((s32 *)&arg1->mapStale[1]) = 1;
        *((s32 *)&arg1->mapStale[3]) = 1;
    default:
        return;
    }
}
