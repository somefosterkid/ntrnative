// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov256/dendou_pc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/dendou_pc.h"
#include "app/dpc_main.h"
#include "app/dpc_seq.h"
#include "gfl/heap.h"
#include "gfl/proc.h"
#include "gfl/std.h"

// The Hall of Fame PC's proc. The ROM doesn't name this file; the name is a guess, after the screen's subject

static BOOL func_ov171_021dc860(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov171_021dc894(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov171_021dc8a8(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's DendouPCProc_Exit
// pokebw2's DendouPCProc_Init
// pokebw2's DendouPCProc_Main
const GameProcFunctions DENDOU_PC_PROC_FUNCTIONS = { func_ov171_021dc860, func_ov171_021dc894, func_ov171_021dc8a8 };

static BOOL func_ov171_021dc860(GameProc *proc, u32 *state, void *param, void *work) {
    DendouPCWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_DENDOU_PC, 0x60000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(DendouPCWork), HEAPID_DENDOU_PC);
    MI_CpuFill8(wk, 0, sizeof(DendouPCWork));
    wk->param = param;
    return TRUE;
}

static BOOL func_ov171_021dc894(GameProc *proc, u32 *state, void *param, void *work) {
    if (func_ov171_021dce70(work) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov171_021dc8a8(GameProc *proc, u32 *state, void *param, void *work) {
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_DENDOU_PC);
    return TRUE;
}
