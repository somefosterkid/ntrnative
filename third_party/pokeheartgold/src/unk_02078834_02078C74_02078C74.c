#include "global.h"

#include "field_system.h"
#include "heap.h"
#include "poke_overlay.h"

FS_EXTERN_OVERLAY(OVY_90);

extern OverlayManagerTemplate _02101270;

void *sub_02078C74(FieldSystem *arg0, enum HeapID arg1, s32 arg2);

void *sub_02078C74(FieldSystem *fieldSystem, enum HeapID heapID, s32 arg2) {
    void *parentWork;

    parentWork = Heap_Alloc(heapID, 0x40U);
    memset(parentWork, 0, 0x40U);
    *(s32 *)((u8 *)parentWork + 0x3C) = arg2;
    *(s32 *)((u8 *)parentWork + 0x38) = 0;
    *(void **)((u8 *)parentWork + 0x34) = fieldSystem->saveData;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    FieldSystem_LaunchApplication(fieldSystem, &_02101270, parentWork);
    return parentWork;
}
