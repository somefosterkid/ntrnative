// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov203/wf2dmap_cmdq.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/wf2dmap/wf2dmap_cmdq.h"
#include "app/wf2dmap/wf2dmap_common.h"
#include "gfl/heap.h"

// A queue of the requests of the Wi-Fi Club's 2D map, named after the ROM's "wf2dmap_cmdq.c"

// A ring buffer with one free slot, so num is one more than the queue holds
struct WF2DMAP_REQCMDQ {
    WF2DMAP_REQCMD *buff;
    u32 num;
    u16 top;
    u16 tail;
};

// pokebw2's WF2DMAP_REQCMDQSysInit
WF2DMAP_REQCMDQ *func_ov127_021c5eec(u32 num, u32 heapId) {
    WF2DMAP_REQCMDQ *queue;

    queue = GFL_HeapAllocate(heapId, sizeof(WF2DMAP_REQCMDQ), TRUE, "wf2dmap_cmdq.c", 211);
    queue->num = num + 1;
    queue->buff = GFL_HeapAllocate(heapId, sizeof(WF2DMAP_REQCMD) * queue->num, TRUE, "wf2dmap_cmdq.c", 216);
    queue->top = 0;
    queue->tail = 0;
    return queue;
}

// pokebw2's WF2DMAP_REQCMDQSysExit
void func_ov127_021c5f30(WF2DMAP_REQCMDQ *queue) {
    GFL_HeapFree(queue->buff);
    GFL_HeapFree(queue);
}

// pokebw2's WF2DMAP_REQCMDQSysCmdPush
void func_ov127_021c5f44(WF2DMAP_REQCMDQ *queue, const WF2DMAP_REQCMD *cmd) {
    WF2DMAP_REQCMD dropped;

    if (queue->top == (queue->tail + 1) % queue->num) {
        func_ov127_021c5f80(queue, &dropped);
    }
    queue->buff[queue->tail] = *cmd;
    queue->tail = (queue->tail + 1) % queue->num;
}

// pokebw2's WF2DMAP_REQCMDQSysCmdPop
BOOL func_ov127_021c5f80(WF2DMAP_REQCMDQ *queue, WF2DMAP_REQCMD *cmd) {
    if (queue->tail == queue->top) {
        return FALSE;
    }
    *cmd = queue->buff[queue->top];
    queue->top = (queue->top + 1) % queue->num;
    return TRUE;
}
