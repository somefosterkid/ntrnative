#ifndef POKEBW2_APP_BATTLE_RECORDER_BR_PROC_SYS_H
#define POKEBW2_APP_BATTLE_RECORDER_BR_PROC_SYS_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/proc.h"
#include "struct_decls.h"

// The Battle Recorder's screens (br_proc_sys.c): a stack of procs, each started with the parameter its before
// function fills from the screen it returns from

#define BR_PROC_SYS_STACK_MAX 6

// The previous screen's ID that a before function gets when the stack was restored from BrProcSysRecovery
#define BR_PROCID_RECOVERY (-2)
// The ID of an empty slot of the stack
#define BR_PROCID_NONE (-1)

// Fills a screen's parameter. preParam and preID are the screen that ran before it
typedef void (*BrProcBeforeFunc)(void *param, void *work, const void *preParam, u32 preID);
// Reads the results from a screen's parameter once it has ended
typedef void (*BrProcAfterFunc)(void *param, void *work);

typedef struct {
    const GameProcFunctions *procFuncs;
    u32 paramSize;
    u32 overlayId;
    BrProcBeforeFunc before_func;
    BrProcAfterFunc after_func;
} BrProcData;

// The stack of screens, kept while a battle video plays
typedef struct {
    u32 stack[BR_PROC_SYS_STACK_MAX];
    u32 stack_num;
} BrProcSysRecovery;

BrProcSys *BrProcSys_Init(u16 procID, const BrProcData *tbl, u16 tbl_max, void *work, BrProcSysRecovery *recovery,
                          HeapID heapId);
void BrProcSys_Exit(BrProcSys *p_wk);
void BrProcSys_Main(BrProcSys *p_wk);
BOOL BrProcSys_IsEnd(BrProcSys *p_wk);
HeapID BrProcSys_GetHeapID(BrProcSys *p_wk);
// Ends the current screen and returns to the one under it
void BrProcSys_Pop(BrProcSys *p_wk);
// Starts a screen on top of the current one
void BrProcSys_Push(BrProcSys *p_wk, u16 procID);
// Saves the stack to the recovery and ends every screen, to play a battle video
void BrProcSys_Interrupt(BrProcSys *p_wk);
// Ends every screen
void BrProcSys_Abort(BrProcSys *p_wk);

#endif // POKEBW2_APP_BATTLE_RECORDER_BR_PROC_SYS_H
