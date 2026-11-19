#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_02_02245B80.h"

void *ov02_02246304(FieldSystem *arg0);

void *ov02_02246304(FieldSystem *arg0) {
    LegendFadeTask *dest;

    dest = Heap_Alloc(HEAP_ID_FIELD1, sizeof(LegendFadeTask));
    if (dest == NULL) {
        GF_AssertFail();
        return NULL;
    }
    MI_CpuFill8(dest, 0U, sizeof(LegendFadeTask));
    dest->fieldSystem = arg0;
    dest->heapID = HEAP_ID_FIELD1;
    return dest;
}
