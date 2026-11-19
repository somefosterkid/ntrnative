#include "global.h"

#include "constants/heap.h"

#include "field/area_data.h"
#include "field/map_prop_animation.h"
#include "field/overlay_01_02204004.h"

#include "assert.h"
#include "error_handling.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "gf_rtc.h"
#include "heap.h"
#include "overlay_01_021FB878.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "field/overlay_01_021FB878.h"

typedef struct MapPropMaterialShape MapPropMaterialShape;

struct AreaDataHeader;

struct AreaDataLoadContext;

struct AreaDataManager;

struct AreaMapAnimation;

struct BicycleSlopeAnimation;

struct BuildModelShapes;

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dRenderObj;

struct Field3dRenderObjManager;

struct MapPropAnimListFile;

struct MapPropAnimation;

struct MapPropAnimationData;

struct MapPropAnimationManager;

struct MapPropAnimationPool;

struct NARC;

struct TimeOfDayPropAnimManager;

struct TimeOfDayPropAnimSet;

typedef struct MapPropAnimationPool MapPropAnimationPool;

typedef void (*MapPropAnimationLoadFunc)(MapPropAnimationPool *pool, MapPropAnimation *animation, void *res, int unused);

typedef struct AreaMapAnimation {
    MapPropAnimationPool *pool;
    MapPropAnimation *mapPropAnimation;
} AreaMapAnimation;

struct MapPropAnimationPool {
    NNSFndAllocator allocator;
    MapPropAnimation *animations;
    MapPropAnimation **freeList;
    MapPropAnimationPool *next;
    MapPropAnimationLoadFunc loadFunc;
    u16 capacity;
    u16 numUsed;
};

typedef struct TimeOfDayPropAnimSet {
    BOOL active;
    int numAnims;
    MapPropAnimation *animations[4];
    NNSG3dRenderObj *renderObj;
} TimeOfDayPropAnimSet;

typedef struct TimeOfDayPropAnimManager {
    TIMEOFDAY timeOfDay;
    int numSets;
    TimeOfDayPropAnimSet sets[4];
} TimeOfDayPropAnimManager;


int sprintf(char *buf, const char *fmt, ...);

extern char ov01_02209AB4[44];

extern char ov01_02209A88[44];

extern const u16 ov01_02208BB0[8];

extern const u16 ov01_02208BA0[8];

void Field3dModel_LoadFromFilesystem(Field3dModel *model, NarcId narcId, s32 fileId, enum HeapID heapID);

void Field3dModelAnimation_LoadFromFilesystem(Field3DModelAnimation *anim, const Field3dModel *model, NarcId narcId, s32 fileId, enum HeapID heapID, NNSFndAllocator *alloc);

MapPropMaterialShape *ov01_021EA724(void);

void ov01_021EA73C(const char *path, MapPropMaterialShape *propMatShp);

void AreaDataManager_Load(AreaDataManager *areaDataManager, void *renderObjManager, MapPropAnimationManager *mapPropAnimationManager, void *areaMapAnimation, void *timeOfDayPropAnims) {
    MapPropAnimListFile animListFile;
    char path[256];
    AreaDataLoadContext *ctx = areaDataManager->loadCtx;
    int i;
    int count;
    s32 animListFileCount;

    areaDataManager->mapPropIdList = AllocAndReadWholeNarcMemberByIdPair(NARC_a_0_4_3, areaDataManager->header.mapPropNarcMemberId, HEAP_ID_FIELD1);
    ctx->mapPropCount = areaDataManager->mapPropIdList[0];
    ((ctx->mapPropCount < 550) ? (void)0 : GF_AssertFail());
    areaDataManager->mapTexFile = AllocAndReadWholeNarcMemberByIdPair(NARC_a_0_4_4, areaDataManager->header.mapTexNarcMemberId, HEAP_ID_FIELD1);
    areaDataManager->mapPropTexFile = AllocAndReadWholeNarcMemberByIdPair(NARC_a_0_7_0, areaDataManager->header.mapPropNarcMemberId, HEAP_ID_FIELD1);
    areaDataManager->mapTex = NNS_G3dGetTex(areaDataManager->mapTexFile);
    if (ctx->mapPropCount) {
        areaDataManager->mapPropTex = NNS_G3dGetTex(areaDataManager->mapPropTexFile);
    } else {
        areaDataManager->mapPropTex = ((void *)0);
    }
    if (!GF3dRender_AllocAndLoadTexResources(areaDataManager->mapTex)) {
        ((0) ? (void)0 : GF_AssertFail());
    }
    ov01_021FB878(areaDataManager->mapTexFile, areaDataManager->mapTex);
    if (areaDataManager->mapPropTex) {
        if (!GF3dRender_AllocAndLoadTexResources(areaDataManager->mapPropTex)) {
            ((0) ? (void)0 : GF_AssertFail());
        }
        ov01_021FB878(areaDataManager->mapPropTexFile, areaDataManager->mapPropTex);
    }
    if (areaDataManager->header.mapPropAnimFileId != 0xFFFF) {
        ov01_0220463C(areaMapAnimation, areaDataManager->header.mapPropAnimFileId);
    }
    for (i = 0; i < 550; i++) {
        areaDataManager->mapPropModels[i] = ((void *)0);
    }
    areaDataManager->buildModelShapes = ((void *)0);
    animListFileCount = MapPropAnimationManager_GetAnimListNARCFileCount(mapPropAnimationManager);
    for (count = 0; count < ctx->mapPropCount; count++) {
        u16 id = areaDataManager->mapPropIdList[count + 1];
        NNSG3dRenderObj *renderObj;

        ((!areaDataManager->mapPropModels[id]) ? (void)0 : GF_AssertFail());
        areaDataManager->mapPropModels[id] = NARC_AllocAndReadWholeMember(ctx->mapPropModelNarc, id, HEAP_ID_FIELD1);
        NARC_ReadWholeMember(areaDataManager->mapPropAnimListNarc, id, &animListFile);
        if (!GF3dRender_BindModelSet(areaDataManager->mapPropModels[id], areaDataManager->mapPropTex)) {
            ((0) ? (void)0 : GF_AssertFail());
        }
        if (!animListFile.filler_3) {
            renderObj = (NNSG3dRenderObj *)Field3dRenderObjManager_AllocRenderObj(renderObjManager, id);
            if (id < animListFileCount) {
                ov01_021E8F3C(id, NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(areaDataManager->mapPropModels[id]), 0), renderObj, areaDataManager->mapPropTex, &animListFile, mapPropAnimationManager, timeOfDayPropAnims);
            }
        }
    }
    if (!areaDataManager->mapPropModels[0]) {
        NNSG3dResTex *tex;

        areaDataManager->mapPropModels[0] = NARC_AllocAndReadWholeMember(ctx->mapPropModelNarc, 0, HEAP_ID_FIELD1);
        Field3dRenderObjManager_AllocRenderObj(renderObjManager, 0);
        tex = NNS_G3dGetTex(areaDataManager->mapPropModels[0]);
        if (tex) {
            if (!GF3dRender_AllocAndLoadTexResources(tex)) {
                ((0) ? (void)0 : GF_AssertFail());
            }
            if (!GF3dRender_BindModelSet(areaDataManager->mapPropModels[0], tex)) {
                ((0) ? (void)0 : GF_AssertFail());
            }
        }
    }
    NARC_Delete(ctx->mapPropModelNarc);
    G3X_EdgeMarking(1);
    if (!areaDataManager->header.lightKind) {
        G3X_SetEdgeColorTable(ov01_02208BA0);
    } else {
        G3X_SetEdgeColorTable(ov01_02208BB0);
    }
    areaDataManager->buildModelShapes = (struct BuildModelShapes *)(ov01_021EA724());
    if (areaDataManager->header.buildModelKind) {
        sprintf(path, ov01_02209A88);
    } else {
        sprintf(path, ov01_02209AB4);
    }
    ov01_021EA73C((const char *)((struct MapPropMaterialShape *)(path)), (struct MapPropMaterialShape *)(areaDataManager->buildModelShapes));
    Heap_Free(areaDataManager->loadCtx);
    areaDataManager->loadCtx = ((void *)0);
}
