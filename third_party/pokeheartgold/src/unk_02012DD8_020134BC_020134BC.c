#include "global.h"

#include "sys_task_api.h"

s32 sub_020131F4(s32, s32);

void sub_020134BC(SysTask *arg0, void *arg1);

void sub_020134BC(SysTask *task, void *arg1) {
    sub_020131F4(*(s32 *)arg1, *(s32 *)((u8 *)arg1 + 4));
    SysTask_Destroy(task);
}

s32 sub_02013220(s32, s32, s32, s32);

void sub_020134D0(SysTask *arg0, void *arg1);

void sub_020134D0(SysTask *task, void *arg1) {
    sub_02013220(*(s32 *)arg1, *(s32 *)((u8 *)arg1 + 4), *(s32 *)((u8 *)arg1 + 8), *(s32 *)((u8 *)arg1 + 0xC));
    SysTask_Destroy(task);
}

s32 sub_020132A8(s32, s32, s32);

void sub_020134EC(SysTask *arg0, void *arg1);

void sub_020134EC(SysTask *task, void *arg1) {
    sub_020132A8(*(s32 *)arg1, *(s32 *)((u8 *)arg1 + 4), *(s32 *)((u8 *)arg1 + 8));
    SysTask_Destroy(task);
}
