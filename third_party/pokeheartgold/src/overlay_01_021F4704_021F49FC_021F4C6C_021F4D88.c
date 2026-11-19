#include "global.h"

#include "field/map_prop.h"

#include "overlay_01_021F4704.h"

void ov01_021F4D88(int index, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, MapLoadManager *mapLoadManager, MapLoadSlot *slot) {
    u32 modelNo;
    u32 header[4];
    u8 buffer[2048];
    modelNo = ov01_021F4A50(header, mapLoadManager, mapMatrix, slot->maps[index], slot->matrixIndices[index], width * height, 0);
    if (modelNo != 0xffff) {
        ov01_021F4AAC(mapLoadManager, slot->maps[index]);
        NARC_ReadFile(mapLoadManager->narc, 0x800, buffer);
        MapPropManager_LoadFromNARC(mapLoadManager->narc, header[3], slot->maps[index]->propManager, mapLoadManager->mapPropAnimationManager);
        ov01_021F4BE8(mapLoadManager, slot->maps[index], header, areaDataManager, slot, modelNo, 0);
    }
    return;
}
