// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov300/zukansearch.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zknsearch_main.h"
#include "app/zknsearch_seq.h"
#include "app/zukansearch.h"
#include "gfl/heap.h"
#include "gfl/proc.h"
#include "gfl/std.h"

// The process of the Pokédex's search screen. The ROM doesn't name this file; zukansearch.c is a guess after
// zknsearch_main.c, the only name the overlay has

static BOOL func_ov210_021bc240(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov210_021bc270(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov210_021bc284(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's ZukanSearch_ProcFunctions
const GameProcFunctions data_ov210_021c0714 = {
    func_ov210_021bc240,
    func_ov210_021bc270,
    func_ov210_021bc284,
};

// pokebw2's ZukanSearch_ProcInit
static BOOL func_ov210_021bc240(GameProc *proc, u32 *state, void *param, void *work) {
    ZknSearchWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_ZUKAN_SEARCH, 0x60000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(ZknSearchWork), HEAPID_ZUKAN_SEARCH);
    MI_CpuFill8(wk, 0, sizeof(ZknSearchWork));
    wk->param = param;
    return TRUE;
}

// pokebw2's ZukanSearch_ProcMain
static BOOL func_ov210_021bc270(GameProc *proc, u32 *state, void *param, void *work) {
    if (func_ov210_021bcf50(work) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ZukanSearch_ProcEnd
static BOOL func_ov210_021bc284(GameProc *proc, u32 *state, void *param, void *work) {
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_ZUKAN_SEARCH);
    return TRUE;
}
