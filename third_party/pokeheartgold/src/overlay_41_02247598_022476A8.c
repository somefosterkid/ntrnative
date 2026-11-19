#include "global.h"

#include "sys_task.h"
#include "systask_environment.h"

s32 ov41_0224AB40(void *);

void ov41_022476A8(u8 *arg0);

void ov41_022476A8(u8 *arg0) {
    ov41_0224AB40(arg0 + 0x568);
}

void ov41_022476E0(SysTask *task, void *data);

void ov41_022476B8(void *arg0, void *arg1);

void ov41_022476B8(void *arg0, void *arg1) {
    void *v0;

    v0 = SysTask_GetData(CreateSysTaskAndEnvironment(ov41_022476E0, 0x10U, 0xAU, HEAP_ID_13));
    *(void **)v0 = arg0;
    *(void **)((u8 *)v0 + 4) = arg1;
    *(s32 *)((u8 *)v0 + 8) = 0;
    *(s32 *)((u8 *)v0 + 0xC) = 0;
}
