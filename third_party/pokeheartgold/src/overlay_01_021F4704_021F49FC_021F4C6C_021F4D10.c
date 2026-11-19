#include "global.h"

#include "field/map_prop.h"

#include "overlay_01_021F4704.h"

void ov01_021F4D10(int index, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, MapLoadManager *mapLoadManager, MapLoadSlot *slot) {
    u32 modelNo;
    u32 header[4];
    modelNo = ov01_021F4A50(header, mapLoadManager, mapMatrix, slot->maps[index], slot->matrixIndices[index], width * height, 1);
    if (modelNo != 0xffff) {
        ov01_021F4AAC(mapLoadManager, slot->maps[index]);
        ov01_021F4AE4(mapLoadManager, slot->maps[index], header[0]);
        MapPropManager_LoadFromNARC(mapLoadManager->narc, header[3], slot->maps[index]->propManager, mapLoadManager->mapPropAnimationManager);
        ov01_021F4BE8(mapLoadManager, slot->maps[index], header, areaDataManager, slot, modelNo, 1);
    }
    return;
}
