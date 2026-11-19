#include "global.h"

#include "sys_task_api.h"
#include "overlay_01_unk_struct_ov101_021d5388_01.h"
#include "overlay_01_021FB4C0.h"

s32 sub_02014A8C(void *);

void ov01_021FB80C(void *arg0);

void ov01_021FB80C(void *arg0) {
    if (*(s32 *)arg0 == 1) {
        sub_02014A8C(*((void **)&((UnkStruct_ov101_021D5388_01 *)arg0)->unk_30C));
        *((s32 *)&((UnkStruct_ov101_021D5388_01 *)arg0)->unk_318) = 1;
    }
}


void ov01_021FB82C(void *arg0);

void ov01_021FB82C(void *arg0) {
    ov01_021FB554(*(void **)((u8 *)arg0 + 0x314));
    *(s32 *)((u8 *)arg0 + 0x314) = 0;
    SysTask_Destroy(*(SysTask **)((u8 *)arg0 + 0x31C));
    *(SysTask **)((u8 *)arg0 + 0x31C) = NULL;
    SysTask_Destroy(*(SysTask **)((u8 *)arg0 + 0x320));
    *(SysTask **)((u8 *)arg0 + 0x320) = NULL;
    SysTask_Destroy(*(SysTask **)((u8 *)arg0 + 0x324));
    *(SysTask **)((u8 *)arg0 + 0x324) = NULL;
    *(s32 *)arg0 = 0;
}
