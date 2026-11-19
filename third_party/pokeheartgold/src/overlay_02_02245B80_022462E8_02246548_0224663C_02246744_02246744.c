#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_02_02245B80.h"

LegendCameraShakeTask *ov02_02246744(FieldSystem *fieldSystem, s32 param_2, s32 param_3, u16 param_4, u16 param_5);

LegendCameraShakeTask *ov02_02246744(FieldSystem *fieldSystem, s32 param_2, s32 param_3, u16 param_4, u16 param_5) {
    LegendCameraShakeTask *dest;

    dest = Heap_Alloc(HEAP_ID_FIELD1, sizeof(LegendCameraShakeTask));
    if (dest == NULL) {
        GF_AssertFail();
        return NULL;
    }
    MI_CpuFill8(dest, 0, sizeof(LegendCameraShakeTask));
    dest->fieldSystem = fieldSystem;
    dest->heapID = HEAP_ID_FIELD1;
    dest->shake = ov02_0224650C(dest->fieldSystem, HEAP_ID_FIELD1);
    ov02_02246548(dest->shake, (s16)param_2, (s16)param_3, param_4, param_5);
    return dest;
}
