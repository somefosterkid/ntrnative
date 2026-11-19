#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "constants/heap.h"

void *ov01_021F3348(u32 arg0);

void ov01_021F336C(void *taskEnv);

void *ov01_021F3348(u32 size) {
    void *v0;

    v0 = Heap_AllocAtEnd(HEAP_ID_FIELD1, size);
    if (v0 == NULL) {
        GF_AssertFail();
    }
    memset(v0, 0, size);
    return v0;
}

void ov01_021F336C(void *taskEnv) {
    Heap_FreeExplicit(HEAP_ID_FIELD1, taskEnv);
}
