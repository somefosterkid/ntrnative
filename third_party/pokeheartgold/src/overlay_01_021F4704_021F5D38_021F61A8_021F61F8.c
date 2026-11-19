#include "global.h"

#include "field/map_prop.h"

#include "field_types_def.h"
#include "heap.h"
#include "overlay_01_021F4704.h"
#include "overlay_01_021FB04C.h"
#include "system.h"

void MapLoadManager_End(MapLoadManager *mapLoadManager);

void MapLoadManager_End(MapLoadManager *mapLoadManager) {
    u8 var_r4;
    u8 var_r6;
    u8 *permissionBytes;

    sub_0201A430();
    NNS_GfdResetLnkTexVramState();
    NNS_GfdResetLnkPlttVramState();
    var_r4 = 0;
    do {
        ov01_021F477C(&mapLoadManager->slots[var_r4]);
        var_r4 += 1;
    } while ((u32)var_r4 < 2U);
    var_r6 = 0;
    do {
        mapLoadManager->maps[var_r6]->modelLoaded = 0;
        ov01_021FB2E8(mapLoadManager->maps[var_r6]->bdhc);
        MapPropManager_Free(mapLoadManager->maps[var_r6]->propManager);
        permissionBytes = mapLoadManager->maps[var_r6]->permissionBytes;
        if (permissionBytes != NULL) {
            Heap_Free(permissionBytes);
            mapLoadManager->maps[var_r6]->permissionBytes = NULL;
        }
        Heap_Free(mapLoadManager->maps[var_r6]);
        mapLoadManager->maps[var_r6] = NULL;
        var_r6 += 1;
    } while ((u32)var_r6 < 4U);
    mapLoadManager->state = MAP_LOAD_STATE_END;
    mapLoadManager->ended = 1;
    mapLoadManager->tracking.enabled = 0;
}
