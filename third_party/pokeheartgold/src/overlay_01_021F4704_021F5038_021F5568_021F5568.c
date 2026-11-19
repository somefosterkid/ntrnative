#include "global.h"

#include "field/map_prop.h"
#include "field/overlay_01_02204004.h"
#include "overlay_01_021F4704.h"
#include "overlay_01_021FB04C.h"

void ov01_021F5568(s32 index, MapLoadManager *mapLoadManager);

void ov01_021F5568(s32 index, MapLoadManager *mapLoadManager) {
    mapLoadManager->maps[index]->modelLoaded = 0;
    ov01_021FB2F4(mapLoadManager->maps[index]->bdhc);
    MapPropManager_Reset(mapLoadManager->maps[index]->propManager);
    mapLoadManager->maps[index]->matrixIndex = -1;
    MIi_CpuClearFast(-1U, mapLoadManager->maps[index]->permissions, sizeof(mapLoadManager->maps[index]->permissions));
    MIi_CpuClear32(0U, &mapLoadManager->maps[index]->soundplates, sizeof(MapSoundplates));
    mapLoadManager->mapStale[index] = 1;
    if (ov01_02204698(mapLoadManager->areaMapAnimation) != 0) {
        ov01_02204688(mapLoadManager->areaMapAnimation, &mapLoadManager->maps[index]->renderObj);
    }
    mapLoadManager->maps[index]->renderObj.anmMat = NULL;
}
