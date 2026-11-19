#include "global.h"

#include "field/area_data.h"
#include "field/map_prop_animation.h"

#include "assert.h"
#include "filesystem.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"

struct AreaDataHeader;

struct AreaDataLoadContext;

struct AreaDataManager;

struct BuildModelShapes;

struct NARC;


void ov01_021EA7E0(BuildModelShapes *shapes);

void AreaDataManager_Free(AreaDataManager **areaDataManager) {
    int i;

    NARC_Delete((*areaDataManager)->mapPropAnimListNarc);
    if ((*areaDataManager)->buildModelShapes != ((void *)0)) {
        ov01_021EA7E0((*areaDataManager)->buildModelShapes);
    }
    for (i = 0; i < 550; i++) {
        if ((*areaDataManager)->mapPropModels[i] != ((void *)0)) {
            Heap_Free((*areaDataManager)->mapPropModels[i]);
        }
    }
    Heap_Free((*areaDataManager)->mapPropIdList);
    Heap_Free((*areaDataManager)->mapTexFile);
    (*areaDataManager)->mapTexFile = ((void *)0);
    Heap_Free((*areaDataManager)->mapPropTexFile);
    (*areaDataManager)->mapPropTexFile = ((void *)0);
    Heap_Free(*areaDataManager);
    *areaDataManager = ((void *)0);
}
