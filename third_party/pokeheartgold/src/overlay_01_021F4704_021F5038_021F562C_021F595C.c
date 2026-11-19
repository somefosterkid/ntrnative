#include "global.h"

#include "overlay_01_map_load_manager.h"

#include "field/area_data.h"
#include "field/area_light.h"
#include "field/field_control.h"
#include "field/map_load_manager.h"
#include "field/map_prop.h"

#include "filesystem.h"
#include "gf_3d_render.h"
#include "heap.h"
#include "map_matrix.h"
#include "overlay_01_021FB878.h"
#include "player_data.h"
#include "safari_zone.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "system.h"

struct AreaDataManager;

struct AreaMapAnimation;

struct Field3dRenderObjManager;

struct MapMatrix;

struct MapPropAnimationManager;

struct MapPropManager;

struct ModelAttributes;

struct NARC;

struct SaveData;

struct SysTask;

struct SysTaskQueue;

typedef struct AreaMapAnimation AreaMapAnimation;

extern const VecFx32 ov01_02206BD8;

extern const MtxFx33 ov01_02206BE4;

void ov01_021F5FB8(const int mapMatrixIndex, const int mapMatrixWidth, const MapMatrix *mapMatrix, VecFx32 *position);

void MapLoadManager_RenderLoadedMap(int mapSlot, MapLoadManager *mapLoadManager, ModelAttributes *modelAttributes);

void MapLoadManager_RenderLoadedMap(int mapSlot, MapLoadManager *mapLoadManager, ModelAttributes *modelAttributes) {
    VecFx32 pos = { 0, 0, 0 };
    VecFx32 scale;
    MtxFx33 rotation;
    LoadedMap *map;

    ov01_021F5FB8(mapLoadManager->maps[mapSlot]->matrixIndex, mapLoadManager->width, mapLoadManager->mapMatrix, &pos);
    if (mapLoadManager->maps[mapSlot]->modelLoaded == TRUE) {
        scale = ov01_02206BD8;
        rotation = ov01_02206BE4;
        GF3dRender_DrawModel(&mapLoadManager->maps[mapSlot]->renderObj, &pos, &rotation, &scale);
    }
    map = mapLoadManager->maps[mapSlot];
    if (map->modelLoaded == TRUE) {
        BOOL lit = ov01_021FBA00(mapLoadManager->areaDataManager);
        ov01_021F3A3C(&pos, mapLoadManager->areaDataManager, lit, modelAttributes, map->propManager);
    }
}
