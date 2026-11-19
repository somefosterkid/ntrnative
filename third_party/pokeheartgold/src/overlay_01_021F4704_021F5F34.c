#include "global.h"
#include "field/map_prop_animation.h"

#include "filesystem.h"
#include "map_matrix.h"
#include "sys_task.h"
#include "overlay_01_map_load_manager.h"

struct AreaDataManager;

struct MapPropManager;

void ov01_021F5D20(MapLoadManager *mapLoadManager);

void ov01_021F5F34(MapLoadManager *mapLoadManager);

s32 ov01_021F5A28(s32, s32, s32);
s32 ov01_021F5A30(s32, s32, s32);
u8 ov01_021F5A54(u8);
u8 ov01_021F6328(s32, s32);
void ov01_021F5F64(s32 arg0, s32 arg1, struct MapLoadManager *arg2);

void ov01_021F5F34(MapLoadManager *mapLoadManager) {
    mapLoadManager->currentSlot = 0;
    mapLoadManager->nextSlot = 0;
    mapLoadManager->numActiveSlots = 0;

    for (int i = 0; i < 2; i++) {
        mapLoadManager->slots[i].active = 0;
        mapLoadManager->slots[i].finished = 1;
    }

    ov01_021F5D20(mapLoadManager);
}

void ov01_021F5F64(s32 arg0, s32 arg1, MapLoadManager *arg2) {
    *((s32 *)&arg2->currentTile) = ov01_021F5A28(arg0, arg1, *((s32 *)&arg2->stride));
    arg2->quadrant = ov01_021F6328(*((s32 *)&arg2->currentTile), *((s32 *)&arg2->stride));
    *((s8 *)&arg2->currentMapSlot) = ov01_021F5A54(arg2->quadrant);
    *((s32 *)&arg2->currentMatrixIndex) = ov01_021F5A30(*((s32 *)&arg2->currentTile), *((s32 *)&arg2->width), *((s32 *)&arg2->stride));
}
