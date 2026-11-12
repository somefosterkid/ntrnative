// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov299/zukanlist.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zknlist_main.h"
#include "app/zknlist_seq.h"
#include "gfl/heap.h"
#include "gfl/proc.h"
#include "gfl/std.h"

// The process of the Pokédex's list. The ROM doesn't name this file; zukanlist.c is a guess after the search screen's
// zukansearch.c

static BOOL func_ov209_021bc240(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov209_021bc274(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov209_021bc288(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's ZukanList_ProcFunctions
const GameProcFunctions data_ov209_021be794 = {
    func_ov209_021bc240,
    func_ov209_021bc274,
    func_ov209_021bc288,
};

// pokebw2's ZukanList_ProcInit
static BOOL func_ov209_021bc240(GameProc *proc, u32 *state, void *param, void *work) {
    ZknListWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_ZUKAN_LIST, 0x60000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(ZknListWork), HEAPID_ZUKAN_LIST);
    MI_CpuFill8(wk, 0, sizeof(ZknListWork));
    wk->param = param;
    return TRUE;
}

// pokebw2's ZukanList_ProcMain
static BOOL func_ov209_021bc274(GameProc *proc, u32 *state, void *param, void *work) {
    if (func_ov209_021bcc58(work) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ZukanList_ProcEnd
static BOOL func_ov209_021bc288(GameProc *proc, u32 *state, void *param, void *work) {
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_ZUKAN_LIST);
    return TRUE;
}
