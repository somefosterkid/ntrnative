#include "global.h"

#include "field_system.h"
#include "heap.h"
#include "overlay_44.h"
#include "work.h"

extern u8 _0210125C[];

extern OverlayManagerTemplate _02101280;

void sub_02078B9C(Work *work, FieldSystem *fieldSystem, enum HeapID heapID, s8 a3);

void sub_02078B9C(Work *work, FieldSystem *fieldSystem, enum HeapID heapID, s8 a3) {
    UnkStruct_ov44_args *args;

    args = Heap_Alloc(heapID, sizeof(UnkStruct_ov44_args));
    args->unk0 = a3;
    args->unk1 = 2;
    args->unk2 = _0210125C[a3];
    args->unk3 = 0;
    args->unk4 = 0;
    args->unk8 = fieldSystem->saveData;
    work->unk10 = args;
    FieldSystem_LaunchApplication(fieldSystem, &_02101280, args);
}
