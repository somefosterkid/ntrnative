#include "global.h"

#include "field/map_prop.h"

#include "heap.h"
#include "overlay_01_021F4704.h"
#include "overlay_01_021FAC44.h"

void ov01_021F497C(MapLoadManager *mapLoadManager);

void ov01_021F497C(MapLoadManager *mapLoadManager) {
    u8 v1;
    LoadedMap *dest;

    v1 = 0;
    do {
        dest = Heap_Alloc(HEAP_ID_FIELD1, sizeof(LoadedMap));
        MI_CpuFill8(dest, 0U, sizeof(LoadedMap));
        mapLoadManager->maps[v1] = dest;
        mapLoadManager->maps[v1]->modelLoaded = 0;
        ov01_021FACE4(v1, mapLoadManager->buffers, (void **)&mapLoadManager->maps[v1]->modelFile);
        mapLoadManager->maps[v1]->propManager = MapPropManager_New(HEAP_ID_FIELD1, mapLoadManager->renderObjManager);
        mapLoadManager->maps[v1]->matrixIndex = -1;
        v1 += 1;
    } while ((u32)v1 < 4U);
}
