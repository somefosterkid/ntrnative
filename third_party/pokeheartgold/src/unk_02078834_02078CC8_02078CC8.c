#include "global.h"

#include "field_system.h"
#include "heap.h"
#include "poke_overlay.h"

FS_EXTERN_OVERLAY(OVY_90);

extern OverlayManagerTemplate _021012A0;

void *sub_02078CC8(FieldSystem *arg0, enum HeapID arg1, s8 arg2);

void *sub_02078CC8(FieldSystem *fieldSystem, enum HeapID heapID, s8 arg2) {
    void *v0;

    v0 = Heap_Alloc(heapID, 0x3CU);
    MI_CpuFill8(v0, 0U, 0x3CU);
    *(s8 *)((u8 *)v0 + 0x38) = arg2;
    *(s8 *)((u8 *)v0 + 0x39) = 0;
    *(void **)((u8 *)v0 + 0x34) = fieldSystem->saveData;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    FieldSystem_LaunchApplication(fieldSystem, &_021012A0, v0);
    return v0;
}
