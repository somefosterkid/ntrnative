// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov172/pdc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Entree Forest capture (overlay 172): the process that overlay 12's scrcmd_entree_forest.c queues, which shows
// the Pokémon of the dream appearing and lets the player throw a Dream Ball at it. The name is the ROM's string, from
// GFL_HeapAllocate's assert. pdc_main.c has the scene and pdc_msg.c the messages

#include "types.h"
#include "app/pdc.h"
#include "app/pdc_main.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "nitro/mi.h"

struct PdcWork {
    GameData *gameData;
    PartyPkm *pkm;
    BtlFieldStatus *status;
    PlayerInfo *playerInfo;
    BagSave *bag;
    TrainerDataSave *trainerData;
    PokeDexSave *pokedex;
    BOOL caught;
};

// The process's work
typedef struct {
    PdcMain *main;
    PdcWork *pdc;
    HeapID heapId;
} PdcProcWork;

static BOOL func_ov098_021b615c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov098_021b61a4(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov098_021b61c4(GameProc *proc, u32 *state, void *param, void *work);

PdcWork *func_ov098_021b6100(GameData *gameData, PartyPkm *pkm, BtlFieldStatus *status, PlayerInfo *playerInfo,
                             BagSave *bag, TrainerDataSave *trainerData, PokeDexSave *pokedex, HeapID heapId) {
    PdcWork *work = GFL_HeapAllocate(heapId, sizeof(PdcWork), FALSE, "pdc.c", 80);

    work->gameData = gameData;
    work->pkm = pkm;
    work->status = status;
    work->playerInfo = playerInfo;
    work->bag = bag;
    work->trainerData = trainerData;
    work->pokedex = pokedex;
    work->caught = FALSE;
    return work;
}

// pokebw2's PdcWork_GetGameData
GameData *func_ov098_021b613c(const PdcWork *work) {
    return work->gameData;
}

// pokebw2's PdcWork_GetPkm
PartyPkm *func_ov098_021b6140(const PdcWork *work) {
    return work->pkm;
}

// pokebw2's PdcWork_GetFieldStatus
BtlFieldStatus *func_ov098_021b6144(const PdcWork *work) {
    return work->status;
}

// pokebw2's PdcWork_GetPlayerInfo
PlayerInfo *func_ov098_021b6148(const PdcWork *work) {
    return work->playerInfo;
}

// pokebw2's PdcWork_GetBag
BagSave *func_ov098_021b614c(const PdcWork *work) {
    return work->bag;
}

// pokebw2's PdcWork_GetTrainerData
TrainerDataSave *func_ov098_021b6150(const PdcWork *work) {
    return work->trainerData;
}

// pokebw2's PdcWork_GetPokedex
PokeDexSave *func_ov098_021b6154(const PdcWork *work) {
    return work->pokedex;
}

// pokebw2's PdcWork_IsCaught
BOOL func_ov098_021b6158(const PdcWork *work) {
    return work->caught;
}

// pokebw2's Pdc_ProcInit
static BOOL func_ov098_021b615c(GameProc *proc, u32 *state, void *param, void *work) {
    PdcWork *pdc = param;
    PdcProcWork *wk;

    GFL_OvlLoad(OVERLAY_ID(94));
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_PDC, 0xc0000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(PdcProcWork), HEAPID_PDC);
    MIi_CpuClearFast(0, wk, sizeof(PdcProcWork));
    wk->heapId = HEAPID_PDC;
    wk->main = func_ov098_021b61e8(pdc, wk->heapId);
    wk->pdc = pdc;
    return TRUE;
}

// pokebw2's PDC_PROC_FUNCTIONS
const GameProcFunctions data_ov098_021b6d74 = {
    func_ov098_021b615c,
    func_ov098_021b61a4,
    func_ov098_021b61c4,
};

// pokebw2's Pdc_ProcMain
static BOOL func_ov098_021b61a4(GameProc *proc, u32 *state, void *param, void *work) {
    PdcProcWork *wk = work;

    wk->pdc->caught = func_ov098_021b6308(wk->main);
    if (wk->pdc->caught) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Pdc_ProcExit
static BOOL func_ov098_021b61c4(GameProc *proc, u32 *state, void *param, void *work) {
    PdcProcWork *wk = work;

    func_ov098_021b632c(wk->main);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_PDC);
    GFL_OvlUnload(OVERLAY_ID(94));
    return TRUE;
}

