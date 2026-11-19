#include "global.h"

#include "error_handling.h"
#include "field_system_def.h"
#include "field_types_def.h"
#include "heap.h"
#include "overlay_02_02245B80.h"

void *ov02_022460CC(FieldSystem *arg0, s16 arg1);

void *ov02_022460CC(FieldSystem *arg0, s16 arg1) {
    LegendEffectTask *dest;

    dest = Heap_Alloc(HEAP_ID_FIELD1, sizeof(LegendEffectTask));
    if (dest == NULL) {
        GF_AssertFail();
        return NULL;
    }
    MI_CpuFill8(dest, 0U, sizeof(LegendEffectTask));
    dest->fieldSystem = arg0;
    dest->effectIdx = arg1;
    dest->heapID = HEAP_ID_FIELD1;
    return dest;
}
