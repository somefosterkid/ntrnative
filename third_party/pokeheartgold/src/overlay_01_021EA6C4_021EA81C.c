#include "global.h"

struct MapPropMaterialShapeIDsLocator;
typedef struct MapPropMaterialShapeIDsLocator MapPropMaterialShapeIDsLocator;
typedef struct MapPropMaterialShapeIDs {
    u16 materialID;
    u16 shapeID;
} MapPropMaterialShapeIDs;
typedef struct MapPropMaterialShape {
    MapPropMaterialShapeIDsLocator *idsLocators;
    MapPropMaterialShapeIDs *ids;
} MapPropMaterialShape;

MapPropMaterialShapeIDs const *ov01_021EA81C(const u16 index, const MapPropMaterialShape *propMatShp);

MapPropMaterialShapeIDs const *ov01_021EA81C(const u16 index, const MapPropMaterialShape *propMatShp) {
    return &propMatShp->ids[index];
}
