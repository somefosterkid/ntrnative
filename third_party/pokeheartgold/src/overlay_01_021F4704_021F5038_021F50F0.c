#include "global.h"

#include "overlay_01_map_load_manager.h"

#include "field/area_data.h"
#include "field/area_light.h"
#include "field/field_control.h"
#include "field/map_load_manager.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/overlay_01_02204004.h"

#include "error_handling.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "gf_3d_render.h"
#include "heap.h"
#include "map_matrix.h"
#include "overlay_01_021FB878.h"
#include "player_data.h"
#include "safari_zone.h"
#include "save.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_02037C94.h"

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

void ov01_021EA3B0(NNSG3dResMdl *model);

enum MapLoadDirection {
    MAP_LOAD_DIR_NONE,
    MAP_LOAD_DIR_EAST,
    MAP_LOAD_DIR_SOUTH,
    MAP_LOAD_DIR_WEST,
    MAP_LOAD_DIR_NORTH,
};

void ov01_021F477C(MapLoadSlot *slot);

BOOL ov01_021F5024(MapLoadTasks *tasks);

void ov01_021F5CB4(MapLoadManager *mapLoadManager);

BOOL ov01_021F5D10(MapLoadManager *mapLoadManager);

void ov01_021F5D20(MapLoadManager *mapLoadManager);

void ov01_021F5D38(u32 direction, MapLoadManager *mapLoadManager);

void ov01_021F5F64(int x, int z, MapLoadManager *mapLoadManager);

void ov01_021F613C(u8 direction, MapLoadManager *mapLoadManager);

void ov01_021F61DC(fx32 x, fx32 z, int *tileX, int *tileZ);

void MapLoadManager_Tick(MapLoadManager *mapLoadManager) {
    MapLoadSlot *slot = &mapLoadManager->slots[mapLoadManager->currentSlot];
    MapLoadTracking *tracking = &mapLoadManager->tracking;
    int x, z;
    LoadedMap *map;
    NNSG3dResMdl *model;

    if (tracking->enabled == 1 && tracking->target != ((void *)0)) {
        if (tracking->moving) {
            ((*tracking->lower <= *tracking->upper) ? (void)0 : GF_AssertFail());
            if (*tracking->upper - *tracking->lower >= ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f)))) {
                ov01_021F61DC(tracking->target->x, tracking->target->z, &x, &z);
                ov01_021F5F64(x, z, mapLoadManager);
                {
                    VecFx32 *dst = &tracking->pos;
                    VecFx32 *src = tracking->target;
                    *dst = *src;
                }
                if (tracking->pos.z % (((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f))) / 2) && sub_02039AD8(1)) {
                    return;
                }
                ((tracking->pos.z % (((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f))) / 2) == 0) ? (void)0 : GF_AssertFail());
                tracking->moving = 0;
                tracking->upper = ((void *)0);
                tracking->lower = ((void *)0);
            }
        } else {
            if (tracking->pos.x != tracking->target->x && tracking->pos.z != tracking->target->z) {
                ((0) ? (void)0 : GF_AssertFail());
            } else if (tracking->pos.x != tracking->target->x || tracking->pos.z != tracking->target->z) {
                tracking->moving = 1;
                if (tracking->pos.x != tracking->target->x) {
                    if (tracking->pos.x > tracking->target->x) {
                        tracking->upper = &tracking->pos.x;
                        tracking->lower = &tracking->target->x;
                        tracking->direction = MAP_LOAD_DIR_WEST;
                    } else {
                        tracking->upper = &tracking->target->x;
                        tracking->lower = &tracking->pos.x;
                        tracking->direction = MAP_LOAD_DIR_EAST;
                    }
                    if (tracking->pos.x % (((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f))) / 2) && sub_02039AD8(1)) {
                        return;
                    }
                    ((tracking->pos.x % (((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f))) / 2) == 0) ? (void)0 : GF_AssertFail());
                } else {
                    if (mapLoadManager->tracking.pos.z > mapLoadManager->tracking.target->z) {
                        tracking->upper = &tracking->pos.z;
                        tracking->lower = &tracking->target->z;
                        tracking->direction = MAP_LOAD_DIR_NORTH;
                    } else {
                        tracking->upper = &tracking->target->z;
                        tracking->lower = &tracking->pos.z;
                        tracking->direction = MAP_LOAD_DIR_SOUTH;
                    }
                    if (tracking->pos.z % (((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f))) / 2) && sub_02039AD8(1)) {
                        return;
                    }
                    ((tracking->pos.z % (((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f))) / 2) == 0) ? (void)0 : GF_AssertFail());
                }
                ov01_021F613C(tracking->direction, mapLoadManager);
            }
        }
    }

    switch (mapLoadManager->state) {
    case MAP_LOAD_STATE_START_CHUNK:
        if (mapLoadManager->mapStale[slot->mapSlots[slot->numStarted]] != 1) {
            mapLoadManager->funcs->loadMap(slot->numStarted, mapLoadManager->areaDataManager, mapLoadManager->mapMatrix, mapLoadManager->width, mapLoadManager->height, mapLoadManager, slot);
        }
        slot->numStarted++;
        mapLoadManager->state = MAP_LOAD_STATE_WAIT_CHUNK;
        break;
    case MAP_LOAD_STATE_WAIT_CHUNK:
        if (mapLoadManager->mapStale[slot->mapSlots[slot->numStarted - 1]] == 1) {
            ov01_021F477C(slot);
            mapLoadManager->mapStale[slot->mapSlots[slot->numStarted - 1]] = 0;
        }
        if (slot->numStarted <= 2 && ov01_021F5024(&slot->tasks) == 1) {
            if (slot->numStarted >= 2) {
                slot->finished = 1;
            } else {
                mapLoadManager->state = MAP_LOAD_STATE_START_CHUNK;
            }
            map = slot->maps[slot->numStarted - 1];
            if (map->modelLoaded == 1) {
                model = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(map->modelFile), 0);
                if (ov01_021FBA00(mapLoadManager->areaDataManager) == 1) {
                    ov01_021EA3B0(model);
                }
            }
            if (mapLoadManager->mapLoadedCallback != ((void *)0)) {
                map = slot->maps[slot->numStarted - 1];
                if (map->matrixIndex >= 0 && map->matrixIndex < mapLoadManager->width * mapLoadManager->height) {
                    mapLoadManager->mapLoadedCallback(mapLoadManager->mapLoadedCallbackArg, map->matrixIndex, map->propManager);
                }
            }
        }
        break;
    case MAP_LOAD_STATE_IDLE:
        break;
    case MAP_LOAD_STATE_END:
        if (ov01_021F5024(&slot->tasks) == 1) {
            mapLoadManager->numActiveSlots = 0;
        }
        break;
    }

    if (mapLoadManager->numActiveSlots == 0) {
        mapLoadManager->currentSlot = 0;
        mapLoadManager->nextSlot = 0;
        mapLoadManager->state = MAP_LOAD_STATE_IDLE;
        return;
    }
    if (ov01_021F5D10(mapLoadManager) == 1) {
        mapLoadManager->slots[mapLoadManager->currentSlot].active = 0;
        ov01_021F5D20(mapLoadManager);
        mapLoadManager->numActiveSlots--;
        mapLoadManager->currentSlot = (mapLoadManager->currentSlot + 1) % 2;
        if (mapLoadManager->numActiveSlots) {
            mapLoadManager->state = MAP_LOAD_STATE_START_CHUNK;
        } else {
            mapLoadManager->currentSlot = 0;
            mapLoadManager->nextSlot = 0;
            mapLoadManager->state = MAP_LOAD_STATE_IDLE;
        }
        if (mapLoadManager->queued.queued == 1) {
            mapLoadManager->queued.queued = 0;
            ov01_021F5D38(mapLoadManager->queued.direction, mapLoadManager);
            ov01_021F5CB4(mapLoadManager);
        }
    }
}
