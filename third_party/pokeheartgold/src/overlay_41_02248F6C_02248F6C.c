#include "global.h"

#include "heap.h"

s32 ov41_02248E28(void **);

void ov41_02248F6C(void **arg0);

void ov41_02248F6C(void **arg0) {
    Heap_Free(*arg0);
    ov41_02248E28(arg0);
}
