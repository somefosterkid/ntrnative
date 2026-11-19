#include "global.h"

#include "field/map_prop.h"

#include "overlay_01_021F4704.h"

void ov01_021F4E88(int matrixIndex, int mapSlot, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, s32 arg6, MapLoadManager *mapLoadManager) {
    u32 modelNo;
    u32 header[4];

    modelNo = ov01_021F4A50(header, mapLoadManager, mapMatrix, mapLoadManager->maps[mapSlot], matrixIndex, width * height, 1);
    if (0xffff != modelNo) {
        ov01_021F4AAC(mapLoadManager, mapLoadManager->maps[mapSlot]);
        ov01_021F4AE4(mapLoadManager, mapLoadManager->maps[mapSlot], header[0]);
        MapPropManager_LoadFromNARC(mapLoadManager->narc, header[3], mapLoadManager->maps[mapSlot]->propManager, mapLoadManager->mapPropAnimationManager);
        ov01_021F4C6C(mapLoadManager, mapLoadManager->maps[mapSlot], header, areaDataManager, modelNo, matrixIndex, arg6, 1);
    }
    return;
}

void ov01_021F4F0C(int matrixIndex, int mapSlot, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, s32 arg6, MapLoadManager *mapLoadManager) {
    u32 modelNo;
    u32 header[4];
    u8 buffer[2048];

    modelNo = ov01_021F4A50(header, mapLoadManager, mapMatrix, mapLoadManager->maps[mapSlot], matrixIndex, width * height, 0);
    if (modelNo != 0xffff) {
        ov01_021F4AAC(mapLoadManager, mapLoadManager->maps[mapSlot]);
        NARC_ReadFile(mapLoadManager->narc, 0x800, buffer);
        MapPropManager_LoadFromNARC(mapLoadManager->narc, header[3], mapLoadManager->maps[mapSlot]->propManager, mapLoadManager->mapPropAnimationManager);
        ov01_021F4C6C(mapLoadManager, mapLoadManager->maps[mapSlot], header, areaDataManager, modelNo, matrixIndex, arg6, 0);
    }
    return;
}
