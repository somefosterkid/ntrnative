#include "global.h"

#include "field_types_def.h"
#include "sys_task.h"
#include "task.h"

void sub_0205AEA8(void *, void *, void *, void *);

void sub_0205AF78(void *, MapObjectManager *);

void sub_0205AD60(SysTask *task, void *param1);

void sub_0205AD60(SysTask *task, void *param1) {
    BOOL BVar1;
    void *uVar2;

    uVar2 = *(void **)param1;
    BVar1 = FieldSystem_TaskIsRunning(*(FieldSystem **)((u8 *)param1 + 0x4d4));
    if (BVar1 == 0) {
        *(u32 *)((u8 *)param1 + 8) = *(u32 *)(*(u8 **)((u8 *)param1 + 0x4d4) + 0x40);
        sub_0205AEA8(param1, uVar2, *(void **)(*(u8 **)((u8 *)param1 + 0x4d4) + 0x3c), *(void **)((u8 *)param1 + 0x4d8));
        sub_0205AF78(param1, *(void **)(*(u8 **)((u8 *)param1 + 0x4d4) + 0x3c));
    }
    return;
}
