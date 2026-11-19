#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_02_02245B80.h"

AlphUnownTextTask *ov02_022467E8(FieldSystem *fieldSystem, u8 msgNo) {
    AlphUnownTextTask *env;

    env = Heap_Alloc(HEAP_ID_FIELD1, sizeof(AlphUnownTextTask));
    if (env == NULL) {
        GF_AssertFail();
        return NULL;
    }
    MI_CpuFill8(env, 0U, sizeof(AlphUnownTextTask));
    env->fieldSystem = fieldSystem;
    env->msgNo = msgNo;
    env->heapID = HEAP_ID_FIELD1;
    return env;
}
