#include "global.h"

#include "field_system.h"
#include "heap.h"
#include "poke_overlay.h"
#include "unk_02078834.h"

FS_EXTERN_OVERLAY(OVY_90);

extern OverlayManagerTemplate _02101260;

UnkAppArgs3C *sub_02078C18(FieldSystem *arg0, enum HeapID arg1, s8 arg2);

UnkAppArgs3C *sub_02078C18(FieldSystem *fieldSystem, enum HeapID heapID, s8 arg2) {
    UnkAppArgs3C *parentWork;

    parentWork = Heap_Alloc(heapID, sizeof(UnkAppArgs3C));
    memset(parentWork, 0, sizeof(UnkAppArgs3C));
    parentWork->unk38 = arg2;
    parentWork->unk39 = 0;
    parentWork->unk34 = fieldSystem->saveData;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    FieldSystem_LaunchApplication(fieldSystem, &_02101260, parentWork);
    return parentWork;
}
