#include "global.h"
#include "field/area_data.h"

struct AreaDataManagerLoadData;
struct MapPropMaterialShape;
typedef struct MapPropMaterialShape MapPropMaterialShape;
typedef struct AreaDataManagerLoadData AreaDataManagerLoadData;
typedef struct AreaDataFile {
    u16 mapPropArchivesID;
    u16 mapTextureArchiveID;

    u16 dummy04;
    u16 areaLightArchiveID;
} AreaDataFile;

NNSG3dResFileHeader **ov01_021FB90C(const int mapPropModelID, AreaDataManager *const areaDataManager);

NNSG3dResFileHeader **ov01_021FB90C(const int mapPropModelID, AreaDataManager *const areaDataManager) {
    ((areaDataManager != 0L) ? (void)0 : GF_AssertFail());

    if (areaDataManager->mapPropModels[mapPropModelID] == 0L) {

        ((areaDataManager->mapPropModels[0] != 0L) ? (void)0 : GF_AssertFail());
        return &areaDataManager->mapPropModels[0];
    }

    return &areaDataManager->mapPropModels[mapPropModelID];
}
