#include "global.h"

#include "camera.h"
#include "heap.h"
#include "overlay_02_02245B80.h"

LegendCameraShake *ov02_0224650C(FieldSystem *fieldSystem, enum HeapID heapID) {
    LegendCameraShake *shake;

    shake = Heap_Alloc(heapID, sizeof(LegendCameraShake));
    MI_CpuFill8(shake, 0U, sizeof(LegendCameraShake));
    shake->heapID = heapID;
    shake->fieldSystem = fieldSystem;
    shake->camera = Camera_New(heapID);
    return shake;
}

void ov02_02246534(LegendCameraShake *shake) {
    Camera_Delete(shake->camera);
    Heap_Free(shake);
}
