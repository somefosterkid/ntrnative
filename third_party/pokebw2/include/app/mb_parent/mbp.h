#ifndef POKEBW2_APP_MB_PARENT_MBP_H
#define POKEBW2_APP_MB_PARENT_MBP_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/mb.h"

// The DS Download Play parent of NitroSDK's demos (mbp.c), as Game Freak adapted it. The names are the demo's, except
// MBP_FreeBuffers and MBP_IsStarted, which are ours

// The parent's states
enum {
    MBP_STATE_STOP,
    MBP_STATE_IDLE,
    MBP_STATE_ENTRY,
    MBP_STATE_DATASENDING,
    MBP_STATE_REBOOTING,
    MBP_STATE_COMPLETE,
    MBP_STATE_CANCEL,
    MBP_STATE_ERROR,
};

void MBP_Init(HeapID heapId, u32 ggid, u32 tgid);
void MBP_Start(const MBGameRegistry *gameInfo, u16 channel);
BOOL MBP_IsBootableAll(void);
void MBP_StartRebootAll(void);
void MBP_Cancel(void);
u16 MBP_GetState(void);
// Frees the file buffer and the library's work
void MBP_FreeBuffers(void);
// Whether the library runs, from MBP_Start until it is ended
BOOL MBP_IsStarted(void);

#endif // POKEBW2_APP_MB_PARENT_MBP_H
