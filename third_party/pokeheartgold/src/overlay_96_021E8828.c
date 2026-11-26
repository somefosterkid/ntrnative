#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "unk_02035900.h"
#include "heap.h"
#include "sys_task_api.h"


void ov96_021E8884(SysTask *task, void *data);

BOOL ov96_021E8828(void *param_1) {
    u16 uVar1;
    uVar1 = *((s16 *)param_1);
    if (uVar1 == 1) {
        uVar1 = sub_0203769C();
        return (u32)uVar1;
    }
    return 0;
}

Ov96Handlers *ov96_021E883C(const CommCmdTable *cmdTable, int cmdCount, void *userData, enum HeapID heapID) {
    Ov96Handlers *handlers;

    handlers = Heap_Alloc(heapID, sizeof(Ov96Handlers));
    memset(handlers, 0, sizeof(Ov96Handlers));
    handlers->cmdTable = cmdTable;
    handlers->cmdCount = cmdCount;
    handlers->userData = userData;
    handlers->task = SysTask_CreateOnVWaitQueue(ov96_021E8884, handlers, 0x1388U);
    return handlers;
}
