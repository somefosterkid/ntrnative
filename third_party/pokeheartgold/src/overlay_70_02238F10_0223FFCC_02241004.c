#include "global.h"

#include "sys_task.h"
#include "systask_environment.h"
#include "unk_02005D10.h"
#include "unk_struct_ov70_02241004.h"

s32 ov70_02240F3C(void *, s32);

void ov70_02241048(SysTask *task, void *data);

void ov70_02241004(UnkStruct_ov70_02241004 *arg0, s32 arg1);

void ov70_02241004(UnkStruct_ov70_02241004 *arg0, s32 arg1) {
    void *v0;

    arg0->unk_11D8 = CreateSysTaskAndEnvironment(ov70_02241048, 0x10U, 5U, HEAP_ID_61);
    v0 = SysTask_GetData(arg0->unk_11D8);
    *(s32 *)v0 = 0;
    *(s32 *)((u8 *)v0 + 4) = 0x8A;
    *(s32 *)((u8 *)v0 + 8) = arg1;
    *(void **)((u8 *)v0 + 0xC) = arg0;
    ov70_02240F3C(v0, 5);
    PlaySE(0x60DU);
}
