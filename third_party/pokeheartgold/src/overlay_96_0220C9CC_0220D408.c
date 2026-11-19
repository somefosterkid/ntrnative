#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

#include "error_handling.h"
#include "sys_task.h"
#include "sys_task_api.h"

u32 ov96_0220D408(UnkStruct_ov96_0220E3F0_6A8 *param_1);

u32 ov96_0220D408(UnkStruct_ov96_0220E3F0_6A8 *param_1) {
    if (param_1 == NULL) {
        GF_AssertFail();
    }
    if (param_1->task == NULL) {
        return 1;
    }
    return 0;
}

void ov96_0220D420(UnkStruct_ov96_0220E3F0_6A8 *param_1, BgConfig *param_2, SpriteSystem *param_3);

void ov96_0220D420(UnkStruct_ov96_0220E3F0_6A8 *param_1, BgConfig *param_2, SpriteSystem *param_3) {
    param_1->unk_4 = param_2;
    param_1->unk_0 = param_3;
    return;
}

void ov96_0220D360(SysTask *arg0, void *arg1);

void ov96_0220D428(UnkStruct_ov96_0220E3F0_6A8 *param_1, u8 param_2, u8 param_3);

void ov96_0220D428(UnkStruct_ov96_0220E3F0_6A8 *param_1, u8 param_2, u8 param_3) {
    SysTask *pSVar1;

    if (param_1->unk_4 == NULL) {
        GF_AssertFail();
    }
    if (param_1->unk_0 == NULL) {
        GF_AssertFail();
    }
    if (param_1->task != NULL) {
        SysTask_Destroy(param_1->task);
    }
    param_1->unk_10 = 0;
    param_1->unk_C = param_2;
    param_1->unk_D = param_3;
    pSVar1 = SysTask_CreateOnMainQueue(ov96_0220D360, param_1, 2);
    param_1->task = pSVar1;
    return;
}
