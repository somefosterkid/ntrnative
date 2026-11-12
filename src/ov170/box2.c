// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov255/box2.c, Black 2/White 2's version of this
// file in overlay 255, adapted to Black's. The names are Black's dsd names and swan's; pokebw2's guesses are in the
// comments.
#include "types.h"
#include "app/box2.h"
#include "app/box2_main.h"
#include "app/box2_seq.h"
#include "gfl/heap.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "save/box.h"

// The PC box's proc. The ROM doesn't name this file; pokebw2 names it after the proc's overlay, box2

BOOL func_ov170_021dc860(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov170_021dc900(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov170_021dc924(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's BOX2_PROC_FUNCTIONS
const GameProcFunctions data_ov170_021ef78c = { func_ov170_021dc860, func_ov170_021dc900, func_ov170_021dc924 };

// pokebw2's Box2Proc_Init. Black 2 calls Box2Main_InitSettings, which also sets unk22 by the mode; Black sets the
// three flags in place
BOOL func_ov170_021dc860(GameProc *proc, u32 *state, void *param, void *work) {
    Box2SysWork *syswk;
    u32 count;
    u16 tray;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_BOX2, 0x10000);
    syswk = GFL_ProcInitSubsystem(proc, sizeof(Box2SysWork), HEAPID_BOX2);
    MI_CpuFill8(syswk, 0, sizeof(Box2SysWork));
    syswk->param = param;
    syswk->tray = BoxSaveAccessor_GetLastOpenedBox(((Box2Param *)param)->boxes);
    syswk->trayMax = BoxSaveAccessor_GetAvailableBoxCount(syswk->param->boxes);
    syswk->getTray = BOX2_GET_NONE;
    syswk->nextSeq = BOX2SEQ_START;

    // Once every box that is open holds a Pokémon, more boxes open
    if (syswk->trayMax != BOX2_TRAY_MAX) {
        if (syswk->trayMax == 8) {
            count = 8;
        } else if (syswk->trayMax == 16) {
            count = 16;
        }
        for (tray = 0; tray < count; tray++) {
            if (howManyPokesInGeneralAreInBox(syswk->param->boxes, tray) == 0) {
                break;
            }
        }
        if (tray == count) {
            syswk->trayMax = BoxSaveAccessor_UnlockMoreBoxes(syswk->param->boxes);
        }
    }

    syswk->unk20 = 0;
    syswk->unk1F = 0;
    syswk->unk21 = 1;
    syswk->procManager = CreateGameProcManager(HEAPID_BOX2);
    return TRUE;
}

// pokebw2's Box2Proc_Main
BOOL func_ov170_021dc900(GameProc *proc, u32 *state, void *param, void *work) {
    Box2SysWork *syswk = work;

    syswk->procMgrResult = GFL_ProcMgrUpdate(syswk->procManager);
    if (func_ov170_021e1e34(syswk, state) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Box2Proc_Exit
BOOL func_ov170_021dc924(GameProc *proc, u32 *state, void *param, void *work) {
    Box2SysWork *syswk = work;

    saveLastOpenedBoxIdx(syswk->param->boxes, syswk->tray);
    FreeGameProcManager(syswk->procManager);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_BOX2);
    return TRUE;
}
