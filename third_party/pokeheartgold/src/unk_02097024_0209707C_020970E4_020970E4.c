#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_manager.h"
#include "unk_struct_0209_c1ec.h"

UnkStruct_0209C194 *sub_02097024(const UnkStruct_0209C194_1 *param_1, enum HeapID param_2);

extern u8 _02108EA0[];

u32 sub_020970E4(UnkStruct_0209C1EC *param_1);

u32 sub_020970E4(UnkStruct_0209C1EC *param_1) {
    UnkStruct_0209C194 *pvVar1;

    pvVar1 = sub_02097024(&param_1->unk_08, HEAP_ID_FIELD2);
    param_1->unk_24 = pvVar1;
    param_1->unk_00 = 1;
    FieldSystem_LaunchApplication(param_1->fieldSystem, (OverlayManagerTemplate *)_02108EA0, param_1->unk_24);
    return 0;
}
