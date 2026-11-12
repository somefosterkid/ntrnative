#include "types.h"
#include "demo/staff_roll.h"
#include "gfl/heap.h"
#include "gfl/proc.h"
#include "gfl/std.h"

// The proc of the credits; the ROM doesn't name this file, the name is a guess

static BOOL func_ov181_021b95a0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov181_021b95d4(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov181_021b95e8(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's StaffRoll_Exit
// pokebw2's StaffRoll_Init
// pokebw2's StaffRoll_Main
// pokebw2's STAFF_ROLL_PROC_FUNCTIONS
const GameProcFunctions data_ov181_021bab24 = { func_ov181_021b95a0, func_ov181_021b95d4, func_ov181_021b95e8 };

static BOOL func_ov181_021b95a0(GameProc *proc, u32 *state, void *param, void *work) {
    StaffRollWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, 0x8e, 0x125000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(StaffRollWork), 0x8e);
    MI_CpuFill8(wk, 0, sizeof(StaffRollWork));
    wk->param = param;
    return TRUE;
}

static BOOL func_ov181_021b95d4(GameProc *proc, u32 *state, void *param, void *work) {
    if (!func_ov181_021b95f8(work)) {
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov181_021b95e8(GameProc *proc, u32 *state, void *param, void *work) {
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x8e);
    return TRUE;
}
