#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_02_02245B80.h"

LegendDemoScene *ov02_02245B9C(FieldSystem *fieldSystem) {
    LegendDemoScene *scene;

    scene = Heap_Alloc(HEAP_ID_FIELD1, sizeof(LegendDemoScene));
    if (scene == NULL) {
        GF_AssertFail();
        return NULL;
    }
    MI_CpuFill8(scene, 0, sizeof(LegendDemoScene));
    scene->fieldSystem = fieldSystem;
    scene->heapID = HEAP_ID_FIELD1;
    return scene;
}
