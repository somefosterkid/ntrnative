#include "global.h"
#include "heap.h"
#include "sys_task_api.h"
#include "pokeathlon/pokeathlon.h"

void ov96_021E8884(SysTask *task, void *param);

void ov96_021E88FC(Ov96Handlers *handlers);

void ov96_021E8884(SysTask *task, void *param) {
    Ov96Handlers *handlers = param;
    s32 i;
    s32 size;
    Ov96Msg *msg;

    i = 0;
    msg = handlers->msgs;
    do {
        if (msg->active == 1) {
            size = msg->size;
            if (size <= 0x26) {
                handlers->cmdTable[msg->cmd].unk_00(0, size, msg->data, handlers->userData);
            } else {
                handlers->cmdTable[msg->cmd].unk_00(0, size, msg->ptr, handlers->userData);
            }
            memset(msg, 0, sizeof(Ov96Msg));
        }
        msg++;
        i++;
    } while (i < 8);
}

void ov96_021E88FC(Ov96Handlers *handlers) {
    SysTask_Destroy(handlers->task);
    Heap_Free(handlers);
}
