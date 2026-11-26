#include "global.h"

#include "field_system.h"
#include "heap.h"
#include "overlay_91_unk_struct_ov115_02260440.h"
#include "poke_overlay.h"

FS_EXTERN_OVERLAY(OVY_90);

extern OverlayManagerTemplate _02101270;

UnkStruct_ov66_02231134 *sub_02078C74(FieldSystem *fieldSystem, enum HeapID heapID, s32 arg2);

UnkStruct_ov66_02231134 *sub_02078C74(FieldSystem *fieldSystem, enum HeapID heapID, s32 arg2) {
    UnkStruct_ov66_02231134 *parentWork;

    parentWork = Heap_Alloc(heapID, sizeof(UnkStruct_ov66_02231134));
    memset(parentWork, 0, sizeof(UnkStruct_ov66_02231134));
    parentWork->unk_3C = arg2;
    parentWork->unk_38 = 0;
    parentWork->saveData = fieldSystem->saveData;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    FieldSystem_LaunchApplication(fieldSystem, &_02101270, parentWork);
    return parentWork;
}
