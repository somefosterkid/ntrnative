#include "global.h"

#include "sys_task_api.h"
#include "overlay_01_unk_struct_ov5_021ef4f8.h"
#include "overlay_01_021FB4C0.h"

s32 sub_02014A8C(void *);

void ov01_021FB80C(UnkStruct_ov5_021EF4F8 *arg0);

void ov01_021FB80C(UnkStruct_ov5_021EF4F8 *arg0) {
    if (arg0->unk_00 == 1) {
        sub_02014A8C(arg0->bufferManager);
        arg0->unk_318 = 1;
    }
}

void ov01_021FB82C(UnkStruct_ov5_021EF4F8 *arg0);

void ov01_021FB82C(UnkStruct_ov5_021EF4F8 *arg0) {
    ov01_021FB554(arg0->unk_314);
    arg0->unk_314 = NULL;
    SysTask_Destroy(arg0->unk_31C);
    arg0->unk_31C = NULL;
    SysTask_Destroy(arg0->unk_320);
    arg0->unk_320 = NULL;
    SysTask_Destroy(arg0->unk_324);
    arg0->unk_324 = NULL;
    arg0->unk_00 = 0;
}
