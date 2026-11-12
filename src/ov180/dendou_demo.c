// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov265/dendou_demo.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "demo/ddemo_main.h"
#include "demo/ddemo_seq.h"
#include "demo/dendou_demo.h"
#include "gfl/heap.h"
#include "gfl/proc.h"
#include "gfl/std.h"

// The Hall of Fame entry demo's process (overlay 265). The file's name is a guess, after dendou_pc.c

static BOOL func_ov180_021b6100(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov180_021b6134(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov180_021b6148(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's DENDOU_DEMO_PROC_FUNCTIONS
// pokebw2's DendouDemo_Exit
// pokebw2's DendouDemo_Init
// pokebw2's DendouDemo_Main
const GameProcFunctions data_ov180_021b800c = { func_ov180_021b6100, func_ov180_021b6134, func_ov180_021b6148 };

static BOOL func_ov180_021b6100(GameProc *proc, u32 *state, void *param, void *work) {
    DDemoWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_DENDOU_DEMO, 0x60000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(DDemoWork), HEAPID_DENDOU_DEMO);
    MI_CpuFill8(wk, 0, sizeof(DDemoWork));
    wk->param = param;
    return TRUE;
}

static BOOL func_ov180_021b6134(GameProc *proc, u32 *state, void *param, void *work) {
    if (func_ov180_021b6a10(work) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov180_021b6148(GameProc *proc, u32 *state, void *param, void *work) {
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_DENDOU_DEMO);
    return TRUE;
}
