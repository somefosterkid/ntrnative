#include "global.h"

#include "error_handling.h"
#include "field_system_def.h"
#include "field_types_def.h"
#include "heap.h"
#include "overlay_02_02245B80.h"

void *ov02_022461AC(FieldSystem *arg0, s8 arg1);

void *ov02_022461AC(FieldSystem *arg0, s8 arg1) {
    LegendWingTask *dest;

    dest = Heap_Alloc(HEAP_ID_FIELD1, sizeof(LegendWingTask));
    if (dest == NULL) {
        GF_AssertFail();
        return NULL;
    }
    MI_CpuFill8(dest, 0U, sizeof(LegendWingTask));
    dest->fieldSystem = arg0;
    dest->wingIdx = arg1;
    dest->heapID = HEAP_ID_FIELD1;
    return dest;
}
