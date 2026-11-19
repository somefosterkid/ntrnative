#include "global.h"

#include "comm_player_manager.h"
#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"

void sub_0205724C(u32, u32, u32);

void sub_02056EA0(u32 param_1);

void sub_02056EA0(u32 param_1) {
    u32 uVar1;

    if (_021D41C4 != NULL) {
        uVar1 = 0;
        do {
            sub_0205724C(uVar1 & 0xff, 0, param_1);
            uVar1 = uVar1 + 1;
        } while ((s32)uVar1 < 8);
        SysTask_Destroy(_021D41C4->task);
        Heap_Free(_021D41C4);
        _021D41C4 = NULL;
    }
    return;
}
