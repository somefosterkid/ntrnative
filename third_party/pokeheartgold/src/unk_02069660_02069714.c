#include "global.h"
#include "simple3dmodel.h"

#include "sys_task.h"
#include "sys_task_api.h"

struct Simple3DModel;
void sub_02069700(SysTask *sysTask, void *_model);

void sub_02069714(Simple3DModel *model);

void sub_02069714(Simple3DModel *model) {
    SysTask *sysTask = SysTask_CreateOnVBlankQueue(sub_02069700, model, 0xffff);
    ((sysTask != 0L) ? (void)0 : GF_AssertFail());
}
