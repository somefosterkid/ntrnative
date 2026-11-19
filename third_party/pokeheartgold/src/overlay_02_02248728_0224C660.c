#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "field_types_def.h"
#include "overlay_01.h"
#include "overlay_02.h"

void *ov02_0224C660(enum HeapID arg0, u32 arg1);

u32 ov02_0224C680(void *param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3);

void *ov02_0224C660(enum HeapID heapID, u32 size) {
    void *v0;

    v0 = Heap_AllocAtEnd(heapID, size);
    if (v0 == NULL) {
        GF_AssertFail();
    }
    memset(v0, 0, size);
    return v0;
}

u32 ov02_0224C680(void *param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3) {
    ov01_022060B8(param_2, 2, 1);
    param_3->state = param_3->state + 1;
    return 0;
}
