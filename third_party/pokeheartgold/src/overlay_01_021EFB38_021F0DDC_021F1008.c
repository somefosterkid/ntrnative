#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_01_021EFB38.h"
#include "overlay_01_unk_struct_ov5_02202120.h"
#include "sys_task_api.h"
#include "unk_02014DA0.h"

s32 ov01_021F1008(void) {
    if (ov01_02209B64 == NULL) {
        GF_AssertFail();
    }
    if (ov01_02209B64->unk_08 == NULL) {
        return 1;
    }
    sub_02015460();
    if (sub_020154B0(ov01_02209B64->unk_08) == 0) {
        return 1;
    }
    return 0;
}

s32 ov01_021F1044(void) {
    if (sub_020154B0(ov01_02209B64->unk_08) == 0) {
        return 1;
    }
    return 0;
}

void ov01_021F1060(void) {
    if (ov01_02209B64 == NULL) {
        GF_AssertFail();
    }
    sub_02014EBC(ov01_02209B64->unk_08);
    Heap_Free(ov01_02209B64->unk_0C);
    ov01_02209B64->unk_08 = NULL;
    ov01_02209B64->unk_0C = NULL;
}

void ov01_021F1148(struct FieldSystem *fieldSystem);

void ov01_021F1094(SysTask *arg0, void *arg1);

void ov01_021F1094(SysTask *task, void *arg1) {
    UnkStruct_ov5_02202120 *v1 = arg1;
    u16 v0;

    v0 = v1->unk_00;
    switch (v0) {
    case 0:
        v1->unk_00 = (u16)(v0 + 1);
        return;
    case 1:
        if (v1->unk_03 == 1) {
            ov01_021F1148(v1->fieldSystem);
            SysTask_Destroy(task);
            v1->unk_00 = (u16)(v1->unk_00 + 1);
        }
        return;
    }
}
