// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_proc_sys.h, the
// lines in use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_PROC_SYS_H
#define BWPORT_APP_BATTLE_RECORDER_BR_PROC_SYS_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The Battle Recorder's screens (br_proc_sys.c): a stack of procs. pokebw2's guessed names are in the comments

// pokebw2's BrProcSys_GetHeapID, BrProcSys_Pop and BrProcSys_Abort
HeapID func_ov186_021f9b9c(BrProcSys *p_wk);
void func_ov186_021f9ba0(BrProcSys *p_wk);
void func_ov186_021f9c6c(BrProcSys *p_wk);

// pokebw2's BrProcSys_Interrupt
// Saves the stack to the recovery and ends every screen, to play a battle video
void func_ov186_021f9c50(BrProcSys *p_wk);

// Black 2's, not checked against Black
#define BR_PROC_SYS_STACK_MAX 6

// The stack of screens, kept while a battle video plays
struct BrProcSysRecovery {
    u32 stack[BR_PROC_SYS_STACK_MAX];
    u32 stack_num;
};

// Black 2's, not checked against Black
// The previous screen's ID that a before function gets when the stack was restored from BrProcSysRecovery
#define BR_PROCID_RECOVERY (-2)

// Black 2's, not checked against Black
// Fills a screen's parameter. preParam and preID are the screen that ran before it
typedef void (*BrProcBeforeFunc)(void *param, void *work, const void *preParam, u32 preID);

// Black 2's, not checked against Black
// Reads the results from a screen's parameter once it has ended
typedef void (*BrProcAfterFunc)(void *param, void *work);

// Black 2's, not checked against Black
struct BrProcData {
    const GameProcFunctions *procFuncs;
    u32 paramSize;
    u32 overlayId;
    BrProcBeforeFunc before_func;
    BrProcAfterFunc after_func;
};

// pokebw2's BrProcSys_Init
BrProcSys *func_ov186_021f99cc(u16 procID, const BrProcData *tbl, u16 tbl_max, void *work, BrProcSysRecovery *recovery,
                          HeapID heapId);

// The ID of an empty slot of the stack
#define BR_PROCID_NONE (-1)

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BrProcSys_Push
// Starts a screen on top of the current one
void func_ov186_021f9bf4(BrProcSys *p_wk, u16 procID);
// pokebw2's BrProcSys_Exit
void func_ov186_021f9a6c(BrProcSys *p_wk);
// pokebw2's BrProcSys_Main
void func_ov186_021f9a80(BrProcSys *p_wk);
// pokebw2's BrProcSys_IsEnd
BOOL func_ov186_021f9b98(BrProcSys *p_wk);

#endif // BWPORT_APP_BATTLE_RECORDER_BR_PROC_SYS_H
