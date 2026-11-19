#include "global.h"

#include "constants/heap.h"

#include "field/map_prop.h"
#include "field/overlay_01_02204004.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_01_021F4704.h"
#include "overlay_01_021FAC44.h"
#include "overlay_01_map_load_manager.h"

void ov01_021F47A0(int x, int z, int width, int height, u32 stride, int *matrixIndices) {
    s32 v0;
    s32 v2;
    s32 v3;
    s32 v4;
    s32 v5;
    s32 v6;
    s32 v8;
    s32 v7;
    u32 v1;

    v8 = (s32)(x + ((u32)(x >> 4) >> 0x1B)) >> 5;
    v7 = (s32)(z + ((u32)(z >> 4) >> 0x1B)) >> 5;
    v0 = height;
    v1 = ov01_021F6328(ov01_021F5A28(x, z, stride), stride);
    v4 = v8 + (v7 * width);
    switch (v1) {
    case 0:
        v6 = v4 - width;
        matrixIndices[3] = v4;
        matrixIndices[0] = (s32)(v6 - 1);
        matrixIndices[1] = v6;
        matrixIndices[2] = (s32)(v4 - 1);
        if ((s32)(v8 - 1) < 0) {
            matrixIndices[0] = -1;
            matrixIndices[2] = -1;
        }
        if ((s32)(v7 - 1) < 0) {
            matrixIndices[1] = -1;
            return;
        }
        return;
    case 1:
        matrixIndices[2] = v4;
        v2 = v4 - width;
        matrixIndices[0] = v2;
        matrixIndices[1] = (s32)(v2 + 1);
        matrixIndices[3] = (s32)(v4 + 1);
        if ((s32)(v8 + 1) >= width) {
            matrixIndices[1] = -1;
            matrixIndices[3] = -1;
        }
        if ((s32)(v7 - 1) < 0) {
            matrixIndices[0] = -1;
            return;
        }
        break;
    case 2:
        matrixIndices[1] = v4;
        matrixIndices[0] = (s32)(v4 - 1);
        v5 = v4 + width;
        matrixIndices[2] = (s32)(v5 - 1);
        matrixIndices[3] = v5;
        if ((s32)(v8 - 1) < 0) {
            matrixIndices[0] = -1;
            matrixIndices[2] = -1;
        }
        if ((s32)(v7 + 1) > v0) {
            matrixIndices[3] = -1;
            return;
        }
        break;
    case 3:
        matrixIndices[0] = v4;
        matrixIndices[1] = (s32)(v4 + 1);
        v3 = v4 + width;
        matrixIndices[2] = v3;
        matrixIndices[3] = (s32)(v3 + 1);
        if ((s32)(v8 + 1) >= width) {
            matrixIndices[1] = -1;
            matrixIndices[3] = -1;
        }
        if ((s32)(v7 + 1) > v0) {
            matrixIndices[0] = -1;
            return;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
}

void ov01_021F488C(MapLoadManager *mapLoadManager, BOOL hasPermissionBytes) {
    LoadedMap *map;
    u32 i;

    i = 0;
    do {
        map = Heap_Alloc(HEAP_ID_FIELD1, sizeof(LoadedMap));
        MI_CpuFill8(map, 0, sizeof(LoadedMap));
        mapLoadManager->maps[i] = map;
        mapLoadManager->maps[i]->modelLoaded = 0;
        ov01_021FACE4(i, mapLoadManager->buffers, (void **)&mapLoadManager->maps[i]->modelFile);
        ov01_021FACEC(i, mapLoadManager->buffers, (void **)&mapLoadManager->maps[i]->bdhcBuffer);
        mapLoadManager->maps[i]->propManager = MapPropManager_New(HEAP_ID_FIELD1, mapLoadManager->renderObjManager);
        mapLoadManager->maps[i]->matrixIndex = 0xffffffff;
        MIi_CpuClearFast(0xffffffff, mapLoadManager->maps[i], sizeof(map->permissions));
        if (hasPermissionBytes != 0) {
            mapLoadManager->maps[i]->permissionBytes = Heap_Alloc(HEAP_ID_FIELD1, 0x400);
            MI_CpuFill8(mapLoadManager->maps[i]->permissionBytes, 0xff, 0x400);
        }
        MIi_CpuClear32(0, &mapLoadManager->maps[i]->soundplates, sizeof(MapSoundplates));
        i = i + 1 & 0xff;
    } while (i < 4);
    return;
}
