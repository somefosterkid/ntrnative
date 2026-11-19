#include "global.h"

#include "field/area_data.h"

#include "overlay_01_021EA6C4.h"


typedef struct MapPropMaterialShapeIDsLocator {
    u16 idsCount;
    u16 idsIndex;
} MapPropMaterialShapeIDsLocator;
typedef struct MapPropMaterialShapeIDs {
    u16 materialID;
    u16 shapeID;
} MapPropMaterialShapeIDs;
typedef struct MapPropMaterialShape {
    MapPropMaterialShapeIDsLocator *idsLocators;
    MapPropMaterialShapeIDs *ids;
} MapPropMaterialShape;
void ov01_021EA804(const int modelID, const MapPropMaterialShape *propMatShp, u16 *idsCount, u16 *idsIndex);

void ov01_021EA7F8(s32 buildModel, AreaDataManager_Sub8AC *a1, u16 *a2) {
    *a2 = *(u16 *)(*(u8 **)a1 + buildModel * 4);
    return;
}

void ov01_021EA804(const int modelID, const MapPropMaterialShape *propMatShp, u16 *idsCount, u16 *idsIndex) {
    *idsCount = propMatShp->idsLocators[modelID].idsCount;
    *idsIndex = propMatShp->idsLocators[modelID].idsIndex;
}
