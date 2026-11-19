#ifndef POKEHEARTGOLD_FIELD_AREA_DATA_H
#define POKEHEARTGOLD_FIELD_AREA_DATA_H

#include "field/map_prop_animation.h"

#include "filesystem.h"

typedef struct BuildModelShapes BuildModelShapes;
typedef BuildModelShapes AreaDataManager_Sub8AC;
typedef struct AreaDataManager AreaDataManager;

typedef struct AreaDataHeader {
    u16 mapPropNarcMemberId;
    u16 mapTexNarcMemberId;
    u16 mapPropAnimFileId;
    u8 buildModelKind;
    u8 lightKind;
} AreaDataHeader;

typedef struct AreaDataLoadContext {
    u32 areaDataBank;
    u32 unk_04;
    NARC *mapPropModelNarc;
    u16 mapPropCount;
    u16 unk_0E;
} AreaDataLoadContext;

// sizeof(AreaDataManager) == 0x8C4 on the DS (550 model pointers at +4)
struct AreaDataManager {
    u32 unk_00;
    NNSG3dResFileHeader *mapPropModels[550];
    void *mapTexFile;
    void *mapPropTexFile;
    NNSG3dResTex *mapTex;
    NNSG3dResTex *mapPropTex;
    BuildModelShapes *buildModelShapes;
    AreaDataHeader header;
    AreaDataLoadContext *loadCtx;
    u16 *mapPropIdList;
    NARC *mapPropAnimListNarc;
};

AreaDataManager *AreaDataManager_Alloc(u8 areaDataBank);
NARC *ov01_021FB904(AreaDataManager *areaDataManager);
NNSG3dResFileHeader **AreaDataManager_GetMapPropModelFile(AreaDataManager *areaDataManager);
void AreaDataManager_Free(AreaDataManager **areaDataManager);
NNSG3dResTex *AreaDataManager_GetMapTexture(AreaDataManager *areaDataManager);
u32 AreaDataManager_GetAreaLightArchiveID(AreaDataManager *areaDataManager);
AreaDataManager_Sub8AC *ov01_021FB9F4(AreaDataManager *areaDataManager);
void AreaDataManager_Load(AreaDataManager *areaDataManager, void *unkC0, MapPropAnimationManager *mapPropAnimationManager, void *unkCC, void *unk104);

#endif // POKEHEARTGOLD_FIELD_AREA_DATA_H
