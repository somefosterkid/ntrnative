#include "global.h"

#include "constants/heap.h"

#include "heap.h"

struct MapPropMaterialShapeIDs;
struct MapPropMaterialShapeIDsLocator;
typedef struct MapPropMaterialShapeIDsLocator MapPropMaterialShapeIDsLocator;
typedef struct MapPropMaterialShapeIDs MapPropMaterialShapeIDs;
typedef struct MapPropMaterialShape {
    MapPropMaterialShapeIDsLocator *idsLocators;
    MapPropMaterialShapeIDs *ids;
} MapPropMaterialShape;

MapPropMaterialShape *ov01_021EA724(void);

struct MapPropMaterialShapeIDsLocator {
    u16 idsCount;
    u16 idsIndex;
};
struct MapPropMaterialShapeIDs {
    u16 materialID;
    u16 shapeID;
};
void ov01_021EA73C(const char *path, MapPropMaterialShape *propMatShp);
void ov01_021EA7E0(MapPropMaterialShape *propMatShp);

MapPropMaterialShape *ov01_021EA724(void) {
    MapPropMaterialShape *propMatShp = Heap_Alloc(HEAP_ID_FIELD1, sizeof(MapPropMaterialShape));
    ((propMatShp != 0L) ? (void)0 : GF_AssertFail());

    return propMatShp;
}

void ov01_021EA73C(const char *path, MapPropMaterialShape *propMatShp) {
    FSFile file;
    FS_InitFile(&file);

    if (FS_OpenFile(&file, path)) {
        u16 idsLocatorsCount;
        int readLength = FS_ReadFile(&file, &idsLocatorsCount, 2);
        ((readLength >= 0) ? (void)0 : GF_AssertFail());

        u16 idsCount;
        readLength = FS_ReadFile(&file, &idsCount, 2);
        ((readLength >= 0) ? (void)0 : GF_AssertFail());

        int idsLocatorsSize = sizeof(MapPropMaterialShapeIDsLocator) * idsLocatorsCount;
        int idsSize = sizeof(MapPropMaterialShapeIDs) * idsCount;

        propMatShp->idsLocators = Heap_Alloc(HEAP_ID_FIELD1, idsLocatorsSize);
        ((propMatShp->idsLocators != 0L) ? (void)0 : GF_AssertFail());

        propMatShp->ids = Heap_Alloc(HEAP_ID_FIELD1, idsSize);
        ((propMatShp->ids != 0L) ? (void)0 : GF_AssertFail());

        readLength = FS_ReadFile(&file, propMatShp->idsLocators, idsLocatorsSize);
        ((readLength >= 0) ? (void)0 : GF_AssertFail());

        readLength = FS_ReadFile(&file, propMatShp->ids, idsSize);
        ((readLength >= 0) ? (void)0 : GF_AssertFail());

        FS_CloseFile(&file);
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }
}

void ov01_021EA7E0(MapPropMaterialShape *propMatShp) {
    Heap_Free(propMatShp->ids);
    Heap_Free(propMatShp->idsLocators);
    Heap_Free(propMatShp);
}
