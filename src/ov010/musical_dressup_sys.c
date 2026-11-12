// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/musical_dressup_sys.c. Black's dressing
// room is one overlay, 133, where Black 2's loads overlays 208 and 209, and its communication is in overlay 132.
#include "types.h"
#include "field/musical.h"
#include "field/musical_dressup_sys.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "save/save_control.h"

typedef struct {
    // The copy of the parameter that overlay 133 gets
    MusicalDressUpParam *dressUp;
    // Overlay 133's work
    void *work;
    u32 result;
    MusicalDressUpParam *param;
} MusicalDressUpWork;

static BOOL func_ov010_02156bd0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov010_02156c28(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov010_02156c4c(GameProc *proc, u32 *state, void *param, void *work);

GameProcFunctions data_ov010_02172724 = {
    func_ov010_02156bd0,
    func_ov010_02156c4c,
    func_ov010_02156c28,
};

static BOOL func_ov010_02156bd0(GameProc *proc, u32 *state, void *param, void *work) {
    MusicalDressUpParam *dressUp = param;
    MusicalDressUpWork *wk;

    *state = 0;
    GFL_OvlLoad(OVERLAY_ID(133));
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_MUSICAL_DRESSUP, 0x40000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(MusicalDressUpWork), HEAPID_MUSICAL_DRESSUP);
    wk->dressUp = GFL_HeapAllocate(HEAPID_MUSICAL_DRESSUP, sizeof(MusicalDressUpParam), FALSE, "musical_dressup_sys.c",
                                   78);
    wk->param = dressUp;
    wk->dressUp->comm = dressUp->comm;
    wk->dressUp->poke = dressUp->poke;
    wk->dressUp->save = dressUp->save;
    return TRUE;
}

static BOOL func_ov010_02156c28(GameProc *proc, u32 *state, void *param, void *work) {
    MusicalDressUpWork *wk = work;

    GFL_HeapFree(wk->dressUp);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_MUSICAL_DRESSUP);
    GFL_OvlUnload(OVERLAY_ID(133));
    return TRUE;
}

static BOOL func_ov010_02156c4c(GameProc *proc, u32 *state, void *param, void *work) {
    MusicalDressUpWork *wk = work;
    u32 result;

    switch (*state) {
    case 0:
        wk->work = func_ov133_021b6100(wk->dressUp, HEAPID_MUSICAL_DRESSUP);
        *state = 1;
        if (wk->param->comm != NULL && func_ov132_021fd904(wk->param->comm) == TRUE) {
            func_ov132_021fd52c(wk->param->comm);
        }
        break;
    case 1:
        result = func_ov133_021b63bc(wk->work);
        if (result != 0) {
            wk->result = result;
            *state = 2;
        }
        break;
    case 2:
        func_ov133_021b6294(wk->work);
        return TRUE;
    }
    return FALSE;
}

MusicalDressUpParam *func_ov010_02156cac(HeapID heapId, MusicalPoke *poke, SaveControl *save) {
    MusicalDressUpParam *param =
        GFL_HeapAllocate(heapId, sizeof(MusicalDressUpParam), FALSE, "musical_dressup_sys.c", 167);

    param->comm = NULL;
    param->save = getAddressOfMusicalDataInfo(save);
    param->poke = poke;
    return param;
}

void func_ov010_02156cd8(MusicalDressUpParam *param) {
    GFL_HeapFree(param);
}
