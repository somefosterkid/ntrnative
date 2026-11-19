#include "global.h"

#include "sys_task_api.h"

s32 sub_020696E8(void *);

void sub_02069700(SysTask *arg0, void *arg1);

void sub_02069700(SysTask *task, void *arg1) {
    sub_020696E8(arg1);
    SysTask_Destroy(task);
}
