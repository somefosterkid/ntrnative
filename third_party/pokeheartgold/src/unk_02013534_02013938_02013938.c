#include "global.h"

#include "heap.h"
#include "unk_02013534.h"

s32 sub_02013FA8(void *arg0);

void sub_02013938(UnkStruct_02013910 *a0) {
    sub_02013FA8(a0);
    Heap_Free(a0);
}
