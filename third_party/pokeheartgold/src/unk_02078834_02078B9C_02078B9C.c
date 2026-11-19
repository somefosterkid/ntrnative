#include "global.h"

#include "field_system.h"
#include "heap.h"

extern s32 _0210125C;

extern OverlayManagerTemplate _02101280;

void sub_02078B9C(void *arg0, FieldSystem *arg1, enum HeapID arg2, s8 arg3);

void sub_02078B9C(void *arg0, FieldSystem *arg1, enum HeapID arg2, s8 arg3) {
    void *temp_r0;

    temp_r0 = Heap_Alloc(arg2, 0xCU);
    *(s8 *)temp_r0 = arg3;
    *(s8 *)((u8 *)temp_r0 + 1) = 2;
    *(u8 *)((u8 *)temp_r0 + 2) = (u8) * (u8 *)(((u8 *)&_0210125C) + arg3);
    *(s8 *)((u8 *)temp_r0 + 3) = 0;
    *(s8 *)((u8 *)temp_r0 + 4) = 0;
    *(void **)((u8 *)temp_r0 + 8) = arg1->saveData;
    *(void **)((u8 *)arg0 + 0x10) = temp_r0;
    FieldSystem_LaunchApplication(arg1, &_02101280, temp_r0);
}
