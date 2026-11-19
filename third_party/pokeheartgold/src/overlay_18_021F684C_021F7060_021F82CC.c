#include "global.h"

#include "heap.h"

void *ov18_021F8168(s32, s32 *);

s32 ov18_021F8764(void *, s32 *, void *, s32, void *, s32, s32, void *);

void ov18_021F82CC(s32 arg0, void *arg1, s32 *arg2, void *arg3, s32 arg4, void *arg5);

void ov18_021F82CC(s32 arg0, void *arg1, s32 *arg2, void *arg3, s32 arg4, void *arg5) {
    s32 v0;
    void *ptr;

    if (arg0 == 0x1A) {
        memcpy(arg1, arg3, arg4 * 2);
        *arg2 = arg4;
        return;
    }
    ptr = ov18_021F8168(arg0 + 7, &v0);
    ov18_021F8764(arg1, arg2, ptr, v0, arg3, arg4, 1, arg5);
    Heap_Free(ptr);
}
