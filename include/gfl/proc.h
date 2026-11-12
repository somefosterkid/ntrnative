// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/proc.h, the lines in use so far.
#ifndef BWPORT_GFL_PROC_H
#define BWPORT_GFL_PROC_H

// Names, layouts and constants from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

typedef BOOL (*GameProcFunc)(GameProc *proc, u32 *state, void *param, void *work);

struct GameProcFunctions {
    GameProcFunc init;
    GameProcFunc main;
    GameProcFunc exit;
};

// A process manager of the game's own, and the update of a frame, which returns 0 once no process is left
GameProcManager *CreateGameProcManager(HeapID heapId);
void FreeGameProcManager(GameProcManager *manager);
BOOL GFL_ProcMgrUpdate(GameProcManager *manager);
// Queues a process to run after the current one
void QueueGameProc(GameProcManager *manager, s32 overlayId, const GameProcFunctions *functions, void *param);
// Allocates the work of a process from the heap, which the process functions get as work
void *GFL_ProcInitSubsystem(GameProc *proc, u32 size, HeapID heapId);
void GFL_ProcReleaseSubsystem(GameProc *proc);

// The game's process manager: queue a process to run after the current one, or replace the current one
void GCTX_ProcMgrQueueProc(s32 overlayId, const GameProcFunctions *functions, void *param);

// The game's process manager (procsys.c), and the update of a frame, which returns 0 once no process is left
void GCTX_ProcMgrInit(HeapID heapId);
u32 GCTX_ProcMgrUpdate(void);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void GCTX_ProcMgrReplaceProc(s32 overlayId, const GameProcFunctions *functions, void *param);

#endif // BWPORT_GFL_PROC_H
