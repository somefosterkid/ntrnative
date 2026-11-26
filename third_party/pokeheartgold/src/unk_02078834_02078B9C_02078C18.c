#include "global.h"

#include "field_system.h"
#include "heap.h"
#include "overlay_91_unk_struct_ov115_02260440.h"
#include "poke_overlay.h"
#include "unk_02078834.h"

FS_EXTERN_OVERLAY(OVY_90);

extern OverlayManagerTemplate _02101260;

UnkStruct_ov115_02260440 *sub_02078C18(FieldSystem *fieldSystem, enum HeapID heapID, s8 arg2);

UnkStruct_ov115_02260440 *sub_02078C18(FieldSystem *fieldSystem, enum HeapID heapID, s8 arg2) {
    UnkStruct_ov115_02260440 *parentWork;

    parentWork = Heap_Alloc(heapID, sizeof(UnkStruct_ov115_02260440));
    memset(parentWork, 0, sizeof(UnkStruct_ov115_02260440));
    parentWork->unk_38 = arg2;
    parentWork->unk_39 = 0;
    parentWork->saveData = fieldSystem->saveData;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    FieldSystem_LaunchApplication(fieldSystem, &_02101260, parentWork);
    return parentWork;
}
