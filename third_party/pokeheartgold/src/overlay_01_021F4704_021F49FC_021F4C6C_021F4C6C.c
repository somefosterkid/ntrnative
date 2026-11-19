#include "global.h"

#include "field/area_data.h"
#include "field/overlay_01_02204004.h"

#include "overlay_01_021F4704.h"
#include "overlay_01_021FB04C.h"

void ov01_021EA3B0(NNSG3dResMdl *model);

NNSG3dResMdl *ov01_021F67B4(NARC *landDataNARC, const int mapModelFileSize, NNSG3dRenderObj *mapRenderObj, NNSG3dResFileHeader **mapModelFile, NNSG3dResTex *mapTexture);

s32 sub_02054E20(s32);

void ov01_021F4C6C(MapLoadManager *mapLoadManager, LoadedMap *map, u32 *header, AreaDataManager *areaDataManager, u32 modelNo, int matrixIndex, BOOL arg6, BOOL loadBdhc) {
    s32 a6 = arg6;
    NNSG3dResMdl *model = ov01_021F67B4(mapLoadManager->narc, header[1], &map->renderObj, &map->modelFile, AreaDataManager_GetMapTexture(areaDataManager));
    if (a6) {
        ov01_021EA3B0(model);
    }
    map->modelLoaded = 1;
    map->matrixIndex = matrixIndex;
    if (loadBdhc != 0) {
        ov01_021FB270(mapLoadManager->narc, header[2], map->bdhc, map->bdhcBuffer);
        if (mapLoadManager->mapLoadedCallback != NULL) {
            mapLoadManager->mapLoadedCallback(mapLoadManager->mapLoadedCallbackArg, matrixIndex, map->propManager);
        }
        if ((ov01_02204698(mapLoadManager->areaMapAnimation) != 0) && (sub_02054E20(modelNo) == 0)) {
            ov01_02204678(mapLoadManager->areaMapAnimation, &map->renderObj);
        }
    }
}
