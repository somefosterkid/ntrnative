#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "linear_interpolation_task.h"
#include "overlay_01_unk_struct_ov5_021de6bc.h"

BOOL ov01_021EFE44(LinearInterpolationTaskS32 *task);

void ov01_021F0960(Window *param0, s32 param1, s32 param2, s32 param3, s32 param4, u8 param5);

BOOL ov01_021F07E0(UnkStruct_ov5_021DE6BC *param0);

UnkStruct_ov5_021DE6BC *ov01_021F0848(enum HeapID arg0);

BOOL ov01_021F07E0(UnkStruct_ov5_021DE6BC *param0) {
    BOOL v0;
    s16 v1, v2, v3, v4;

    if (param0->unk_2E == 0) {
        return 1;
    }

    v0 = ov01_021EFE44(&param0->unk_00);
    ov01_021EFE44(&param0->unk_14);

    v3 = param0->unk_00.currentValue - (param0->unk_2C / 2);
    v1 = param0->unk_14.currentValue - (param0->unk_2D / 2);
    v4 = v3 + param0->unk_2C;
    v2 = v1 + param0->unk_2D;

    ov01_021F0960(param0->unk_28, v1, v2, v3, v4, param0->unk_2F);

    return v0;
}

UnkStruct_ov5_021DE6BC *ov01_021F0848(enum HeapID heapID) {
    UnkStruct_ov5_021DE6BC *v0;

    v0 = Heap_Alloc(heapID, sizeof(UnkStruct_ov5_021DE6BC));
    memset(v0, 0, sizeof(UnkStruct_ov5_021DE6BC));
    return v0;
}
