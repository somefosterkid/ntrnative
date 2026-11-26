#include "global.h"

#include "field_system.h"
#include "heap.h"
#include "overlay_91_unk_struct_ov115_02260440.h"
#include "poke_overlay.h"

FS_EXTERN_OVERLAY(OVY_90);

extern OverlayManagerTemplate _021012A0;

UnkStruct_ov115_02260440 *sub_02078CC8(FieldSystem *fieldSystem, enum HeapID heapID, s8 arg2);

UnkStruct_ov115_02260440 *sub_02078CC8(FieldSystem *fieldSystem, enum HeapID heapID, s8 arg2) {
    UnkStruct_ov115_02260440 *args;

    args = Heap_Alloc(heapID, sizeof(UnkStruct_ov115_02260440));
    MI_CpuFill8(args, 0U, sizeof(UnkStruct_ov115_02260440));
    args->unk_38 = arg2;
    args->unk_39 = 0;
    args->saveData = fieldSystem->saveData;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_90), OVY_LOAD_ASYNC);
    FieldSystem_LaunchApplication(fieldSystem, &_021012A0, args);
    return args;
}
