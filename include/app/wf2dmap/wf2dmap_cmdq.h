// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wf2dmap/wf2dmap_cmdq.h: the lines in use
// so far.
#ifndef BWPORT_APP_WF2DMAP_WF2DMAP_CMDQ_H
#define BWPORT_APP_WF2DMAP_WF2DMAP_CMDQ_H

#include "types.h"
#include "app/wf2dmap/wf2dmap_common.h"

typedef struct WF2DMAP_REQCMDQ WF2DMAP_REQCMDQ;

// pokebw2's WF2DMAP_REQCMDQSysInit
// A queue that holds num requests
WF2DMAP_REQCMDQ *func_ov127_021c5eec(u32 num, u32 heapId);
// pokebw2's WF2DMAP_REQCMDQSysCmdPop
// Returns FALSE when the queue is empty
BOOL func_ov127_021c5f80(WF2DMAP_REQCMDQ *queue, WF2DMAP_REQCMD *cmd);
// pokebw2's WF2DMAP_REQCMDQSysCmdPush
// Drops the oldest request when the queue is full
void func_ov127_021c5f44(WF2DMAP_REQCMDQ *queue, const WF2DMAP_REQCMD *cmd);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WF2DMAP_REQCMDQSysExit
void func_ov127_021c5f30(WF2DMAP_REQCMDQ *queue);

#endif // BWPORT_APP_WF2DMAP_WF2DMAP_CMDQ_H
