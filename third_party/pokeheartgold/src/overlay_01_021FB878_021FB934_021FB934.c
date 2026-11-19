#include "global.h"

#include "field/area_data.h"

#include "error_handling.h"

NNSG3dResFileHeader **AreaDataManager_GetMapPropModelFile(AreaDataManager *areaDataManager) {
    if (areaDataManager == NULL) {
        GF_AssertFail();
    }
    return areaDataManager->mapPropModels;
}
