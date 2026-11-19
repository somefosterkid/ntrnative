#include "global.h"

#include "overlay_01_map_load_manager.h"

#include "constants/heap.h"

#include "field/area_data.h"
#include "field/area_light.h"
#include "field/field_control.h"
#include "field/map_load_manager.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/overlay_01_02204004.h"

#include "dex_mon_measures.h"
#include "error_handling.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_3d_render.h"
#include "heap.h"
#include "map_matrix.h"
#include "player_data.h"
#include "safari_zone.h"
#include "save.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "system.h"

struct AreaDataManager;

struct AreaMapAnimation;

struct AsyncWriteManager;

struct BicycleSlopeAnimation;

struct Field3dRenderObj;

struct Field3dRenderObjManager;

struct MAPDATA;

struct MapMatrix;

struct MapMatrixData;

struct MapPropAnimation;

struct MapPropAnimationData;

struct MapPropAnimationManager;

struct MapPropAnimationPool;

struct MapPropManager;

struct NARC;

struct PokedexData;

struct SaveArrayHeader;

struct SaveData;

struct SaveSlotSpec;

struct SysTask;

struct SysTaskQueue;

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

MapLoadBuffers *ov01_021FAC44(BOOL withSecondary);

void LoadSlotMap_Default(int index, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, MapLoadManager *mapLoadManager, MapLoadSlot *slot);

void LoadSlotMap_NoCollision(int index, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, MapLoadManager *mapLoadManager, MapLoadSlot *slot);

void LoadSlotMap_SafariZone(int index, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, MapLoadManager *mapLoadManager, MapLoadSlot *slot);

void InitialLoad_Default(MapLoadManager *mapLoadManager, int x, int z, u32 stride);

void InitialLoad_NoCollision(MapLoadManager *mapLoadManager, int x, int z, u32 stride);

void InitialLoad_SafariZone(MapLoadManager *mapLoadManager, int x, int z, u32 stride);

extern const MapLoadFuncs ov01_02206BC0;

extern const MapLoadFuncs ov01_02206BD0;

extern const MapLoadFuncs ov01_02206BC8;

void ov01_021F5F34(MapLoadManager *mapLoadManager);

MapLoadManager *MapLoadManager_New(MapMatrix *mapMatrix, void *areaDataManager, void *renderObjManager, MapPropAnimationManager *mapPropAnimationManager, void *areaMapAnimation, int mode, SaveData *saveData) {
    MapLoadManager *mapLoadManager = Heap_Alloc(HEAP_ID_FIELD1, sizeof(MapLoadManager));
    BOOL withSecondary;

    MI_CpuFill8(mapLoadManager, 0, sizeof(MapLoadManager));
    if (mode == 0) {
        mapLoadManager->funcs = &ov01_02206BC0;
        withSecondary = 1;
    } else if (mode == 1) {
        mapLoadManager->funcs = &ov01_02206BD0;
        withSecondary = 0;
    } else if (mode == 2) {
        mapLoadManager->funcs = &ov01_02206BC8;
        withSecondary = 1;
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }
    mapLoadManager->saveData = saveData;
    mapLoadManager->buffers = ov01_021FAC44(withSecondary);
    mapLoadManager->areaDataManager = areaDataManager;
    mapLoadManager->mapMatrix = mapMatrix;
    mapLoadManager->renderObjManager = renderObjManager;
    mapLoadManager->width = MapMatrix_GetWidth(mapMatrix);
    mapLoadManager->height = MapMatrix_GetHeight(mapMatrix);
    mapLoadManager->stride = mapLoadManager->width * 32;
    mapLoadManager->mapPropAnimationManager = mapPropAnimationManager;
    mapLoadManager->areaMapAnimation = areaMapAnimation;
    mapLoadManager->tracking.enabled = 1;
    ov01_021F5F34((MapLoadManager *)(mapLoadManager));
    mapLoadManager->ended = 0;
    mapLoadManager->state = MAP_LOAD_STATE_IDLE;
    mapLoadManager->narc = NARC_New(NARC_fielddata_landdata_land_data, HEAP_ID_FIELD1);
    mapLoadManager->mapLoadedCallback = ((void *)0);
    return mapLoadManager;
}
