// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov182/dwc_raputil.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/dwc_raputil.h"
#include "app/dwc_utility.h"
#include "dwc/utility.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "nitro/os.h"
#include "nitro/os.h"
#include "system/wih_dwc.h"

// The game's procs around NitroDWC's Wi-Fi Connection settings (dwc_raputil.c, from the string of its allocation),
// which the start menu and the Game Sync event open. The rest of the overlay is the library's

void func_ov016_021b6100(HeapID heapId);
BOOL func_ov016_021b6154(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov016_021b6178(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov016_021b6184(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov016_021b6190(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's DWC_UTILITY_RESET_PROC_FUNCTIONS
const GameProcFunctions data_ov016_021d77ac = {
    func_ov016_021b6154,
    func_ov016_021b6178,
    func_ov016_021b6184,
};

// pokebw2's DWC_UTILITY_PROC_FUNCTIONS
const GameProcFunctions data_ov016_021d77b8 = {
    func_ov016_021b6154,
    func_ov016_021b6178,
    func_ov016_021b6190,
};

// Runs the settings with interrupts off, which a DSi skips
// pokebw2's DwcRapUtil_StartUtility
void func_ov016_021b6100(HeapID heapId) {
    u32 mask = OS_DisableInterrupts();
    void *work = GFL_HeapAllocate(heapId, DWC_UTILITY_WORK_SIZE, TRUE, "dwc_raputil.c", 61);

    if (!OS_IsRunOnTwl()) {
        DWC_StartUtility(work, DWC_LANGUAGE_ENGLISH, DWC_UTILITY_TOP_MENU_FOR_USA);
    }
    GFL_HeapFree(work);
    OS_RestoreInterrupts(mask);
    OS_EnableIrq();
}

BOOL func_ov016_021b6154(GameProc *proc, u32 *state, void *param, void *work) {
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_DWC_UTILITY, 0x40100);
    func_0200f17c(GFL_NET_TYPE_WIFI);
    GFL_OvlLoad(OVERLAY_ID(107));
    return TRUE;
}


// Turns the settings off, for the start menu and the Game Sync event
// Turns the settings off, for the start menu and the Game Sync event
static BOOL func_ov016_021b6178(GameProc *proc, u32 *state, void *param, void *work) {
    func_ov016_021b6100(HEAPID_DWC_UTILITY);
    return TRUE;
}


// Restarts the game, for the start menu
// pokebw2's DwcRapUtil_ResetProcExit
BOOL func_ov016_021b6184(GameProc *proc, u32 *state, void *param, void *work) {
    OS_ResetSystem(0);
    return TRUE;
}

static BOOL func_ov016_021b6190(GameProc *proc, u32 *state, void *param, void *work) {
    func_0200f234(GFL_NET_TYPE_WIFI);
    GFL_OvlUnload(OVERLAY_ID(107));
    GFL_HeapDelete(HEAPID_DWC_UTILITY);
    downloadPlayConfigForWifi();
    return TRUE;
}


