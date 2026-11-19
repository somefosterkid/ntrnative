#include "global.h"

#include "sys_task.h"

void Heap_Free(void *ptr);

void ov96_021FBE3C(void *ptr);

void ov96_021FBE3C(void *ptr) {
    Heap_Free(ptr);
}

s32 ov96_021FBE44(s32 param_1, s32 param_2);

s32 ov96_021FBE44(s32 param_1, s32 param_2) {
    return param_1 + param_2 * 0x10;
}
