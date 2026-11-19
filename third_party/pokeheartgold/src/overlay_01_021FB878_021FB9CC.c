#include "global.h"

#include "field/area_data.h"
#include "field/overlay_01_021FB878.h"

#include "error_handling.h"
#include "overlay_01_021FB878.h"


NNSG3dResTex *AreaDataManager_GetMapTexture(AreaDataManager *areaDataManager) {
    if (areaDataManager == NULL) {
        GF_AssertFail();
    }
    return areaDataManager->mapTex;
}

NNSG3dResTex *ov01_021FB9E0(AreaDataManager *areaDataManager) {
    if (areaDataManager == NULL) {
        GF_AssertFail();
    }
    return areaDataManager->mapPropTex;
}

AreaDataManager_Sub8AC *ov01_021FB9F4(AreaDataManager *areaDataManager) {
    return areaDataManager->buildModelShapes;
}

s32 ov01_021FBA00(AreaDataManager *arg0) {
    if (arg0->header.lightKind != 0) {
        return 1;
    }
    return 0;
}

u32 AreaDataManager_GetAreaLightArchiveID(AreaDataManager *areaDataManager) {
    u8 v0;

    v0 = areaDataManager->header.lightKind;
    switch (v0) {
    case 0:
        return 1U;
    case 1:
        return 0U;
    case 2:
        return 3U;
    default:
        return 0U;
    }
}
