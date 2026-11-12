// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov216/guru2.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/guru2.h"
#include "app/p_status.h"
#include "app/pokelist.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "save/pokedex.h"
#include "system/game_data.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
BOOL func_ov137_021de7fc(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov137_021dce10(GameProc *proc, u32 *state, void *param, void *work);
extern GameProcFunctions data_ov091_021bed00;
extern GameProcFunctions data_ov131_021dc800;
PokeListParam *PokeListParam_Create(GameData *gameData, u32 mode, PokeParty *party, HeapID heapId);

// guru2.c, which the ROM names: the proc of the Union Room's Spin Trade. It runs the receipt screen, where the group
// gathers, the party list on which each member picks an Egg, and the main screen with the disc. Names are ours

typedef struct Guru2CallWork Guru2CallWork;

// Black 2's; twinned code confirms the offsets of seq, param, g2p, gameData and procManager, and its size in Black
struct Guru2CallWork {
    int seq;
    int partyIndex;
    Guru2Param *param;
    Guru2ProcWork *g2p;
    GameData *gameData;
    u32 unk14;
    PokeListParam *pokeList;
    PStatusParam *pokeStatus;
    GameProcManager *procManager;
};

enum {
    GURU2_SEQ_START_RECEIPT,
    GURU2_SEQ_WAIT_RECEIPT,
    GURU2_SEQ_WAIT_POKE_LIST,
    GURU2_SEQ_WAIT_POKE_STATUS,
    GURU2_SEQ_WAIT_MAIN,
    GURU2_SEQ_END,
};

// The party list's mode for picking an Egg
#define GURU2_POKELIST_MODE 0x15

static BOOL func_ov137_021dc860(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov137_021dc888(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov137_021dc8bc(GameProc *proc, u32 *state, void *param, void *work);
static Guru2ProcWork *func_ov137_021dc8d0(Guru2Param *param, u32 heapId);
static void func_ov137_021dc90c(Guru2ProcWork *g2p);
static BOOL func_ov137_021dc924(Guru2ProcWork *g2p);
static BOOL func_ov137_021dc928(Guru2CallWork *wk);
static PokeListParam *func_ov137_021dc950(Guru2CallWork *wk, u32 mode, u32 index);
static BOOL func_ov137_021dc9ac(Guru2CallWork *wk);
static BOOL func_ov137_021dc9f4(Guru2CallWork *wk);
static BOOL func_ov137_021dcab4(Guru2CallWork *wk);
static BOOL func_ov137_021dcae8(Guru2CallWork *wk);
static BOOL func_ov137_021dcaf0(Guru2CallWork *wk);

// pokebw2's sMainProcFunctions
static const GameProcFunctions data_ov137_021e1a44 = {
    func_ov137_021de7fc,
    func_ov137_021dea0c,
    func_ov137_021de960,
};

// pokebw2's sReceiptProcFunctions
static const GameProcFunctions data_ov137_021e1a38 = {
    func_ov137_021dce10,
    func_ov137_021dcf80,
    func_ov137_021dd0b8,
};

// pokebw2's sGuru2SeqFuncs
static BOOL (*const data_ov137_021e1a50[])(Guru2CallWork *wk) = {
    func_ov137_021dc928,
    func_ov137_021dc9ac,
    func_ov137_021dc9f4,
    func_ov137_021dcab4,
    func_ov137_021dcae8,
    func_ov137_021dcaf0,
};

// pokebw2's GURU2_PROC_FUNCTIONS
const GameProcFunctions data_ov137_021e1a2c = {
    func_ov137_021dc860,
    func_ov137_021dc888,
    func_ov137_021dc8bc,
};

// pokebw2's Guru2Proc_Init
static BOOL func_ov137_021dc860(GameProc *proc, u32 *state, void *param, void *work) {
    Guru2Param *g2param = param;
    Guru2CallWork *wk;

    wk = GFL_ProcInitSubsystem(proc, sizeof(Guru2CallWork), HEAPID_GAMEEVENT);
    MI_CpuFill8(wk, 0, sizeof(Guru2CallWork));
    wk->gameData = g2param->gameData;
    wk->param = g2param;
    wk->procManager = CreateGameProcManager(HEAPID_USER);
    return TRUE;
}

// pokebw2's Guru2Proc_Main
static BOOL func_ov137_021dc888(GameProc *proc, u32 *state, void *param, void *work) {
    Guru2CallWork *wk = work;

    switch (GFL_ProcMgrUpdate(wk->procManager)) {
    case 0:
        if (data_ov137_021e1a50[wk->seq](wk) == TRUE) {
            return TRUE;
        }
        break;
    case 1:
        break;
    case 2:
        break;
    }
    return FALSE;
}

// pokebw2's Guru2Proc_Exit
static BOOL func_ov137_021dc8bc(GameProc *proc, u32 *state, void *param, void *work) {
    Guru2CallWork *wk = work;

    FreeGameProcManager(wk->procManager);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's Guru2ProcWork_Create
static Guru2ProcWork *func_ov137_021dc8d0(Guru2Param *param, u32 heapId) {
    Guru2ProcWork *g2p;

    g2p = GFL_HeapAllocate(heapId, sizeof(Guru2ProcWork), FALSE, "guru2.c", 0xab);
    MI_CpuFill8(g2p, 0, sizeof(Guru2ProcWork));
    g2p->param = param;
    g2p->g2c = func_ov137_021dcb04(g2p, heapId);
    return g2p;
}

// pokebw2's Guru2ProcWork_Free
static void func_ov137_021dc90c(Guru2ProcWork *g2p) {
    func_ov137_021dcb90(g2p->g2c);
    func_ov137_021dcb60(g2p->g2c);
    GFL_HeapFree(g2p);
}

// pokebw2's Guru2ProcWork_IsReceiptDone
static BOOL func_ov137_021dc924(Guru2ProcWork *g2p) {
    return g2p->receiptDone;
}

// pokebw2's Guru2Seq_StartReceipt
static BOOL func_ov137_021dc928(Guru2CallWork *wk) {
    wk->g2p = func_ov137_021dc8d0(wk->param, HEAPID_GAMEEVENT);
    wk->seq = GURU2_SEQ_WAIT_RECEIPT;
    QueueGameProc(wk->procManager, OVERLAY_NONE, &data_ov137_021e1a38, wk->g2p);
    return FALSE;
}

// pokebw2's Guru2_CreatePokeListParam
static PokeListParam *func_ov137_021dc950(Guru2CallWork *wk, u32 mode, u32 index) {
    PokeListParam *pokeList;
    int i;

    pokeList = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(PokeListParam), TRUE, __FILE__, 0x100);
    pokeList->party = GameData_GetParty(wk->gameData);
    pokeList->bag = GameData_GetBag(wk->gameData);
    pokeList->unk38 = 0;
    pokeList->mode = mode;
    pokeList->index = index;
    for (i = 0; i < PokeParty_GetPkmCount(pokeList->party); i++) {
        PokeParty_GetPkm(pokeList->party, i);
    }
    return pokeList;
}

// pokebw2's Guru2Seq_WaitReceipt
static BOOL func_ov137_021dc9ac(Guru2CallWork *wk) {
    if (!func_ov137_021dc924(wk->g2p)) {
        wk->seq = GURU2_SEQ_END;
    } else {
        func_0203d7b0(TRUE);
        wk->pokeList = func_ov137_021dc950(wk, GURU2_POKELIST_MODE, 0);
        QueueGameProc(wk->procManager, OVERLAY_ID(91), &data_ov091_021bed00, wk->pokeList);
        wk->g2p->state = GURU2_PROC_STATE_POKE_LIST;
        wk->seq = GURU2_SEQ_WAIT_POKE_LIST;
    }
    return FALSE;
}

// pokebw2's Guru2Seq_WaitPokeList
static BOOL func_ov137_021dc9f4(Guru2CallWork *wk) {
    u32 result = wk->pokeList->result;
    u32 index = wk->pokeList->index;

    GFL_HeapFree(wk->pokeList);
    if (result == 1) {
        PokeDexSave *pokedex;
        PokeParty *party;

        GetGameDataPlayerInfo(wk->gameData);
        pokedex = GameData_GetPokedex(wk->gameData);
        wk->pokeStatus = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(PStatusParam), FALSE, __FILE__, 0x148);
        MI_CpuFill8(wk->pokeStatus, 0, sizeof(PStatusParam));
        party = GameData_GetParty(wk->gameData);
        wk->pokeStatus->party = party;
        wk->pokeStatus->gameData = wk->gameData;
        wk->pokeStatus->dataType = 1;
        wk->pokeStatus->partyCount = PokeParty_GetPkmCount(party);
        wk->pokeStatus->partyIndex = 0;
        wk->pokeStatus->mode = PSTATUS_MODE_1;
        wk->pokeStatus->move = 0;
        wk->pokeStatus->page = PSTATUS_PAGE_INFO;
        wk->pokeStatus->isNationalDex = PokeDex_IsNationalObtained(pokedex);
        wk->partyIndex = index;
        wk->pokeStatus->partyIndex = index;
        QueueGameProc(wk->procManager, OVERLAY_PSTATUS, &data_ov131_021dc800, wk->pokeStatus);
        wk->seq = GURU2_SEQ_WAIT_POKE_STATUS;
    } else {
        wk->g2p->partyIndex = index;
        QueueGameProc(wk->procManager, OVERLAY_NONE, &data_ov137_021e1a44, wk->g2p);
        wk->g2p->state = GURU2_PROC_STATE_MAIN;
        wk->seq = GURU2_SEQ_WAIT_MAIN;
    }
    return FALSE;
}

// pokebw2's Guru2Seq_WaitPokeStatus
static BOOL func_ov137_021dcab4(Guru2CallWork *wk) {
    u8 index = wk->pokeStatus->partyIndex;

    GFL_HeapFree(wk->pokeStatus);
    wk->pokeList = func_ov137_021dc950(wk, GURU2_POKELIST_MODE, index);
    QueueGameProc(wk->procManager, OVERLAY_ID(91), &data_ov091_021bed00, wk->pokeList);
    wk->seq = GURU2_SEQ_WAIT_POKE_LIST;
    return FALSE;
}

// pokebw2's Guru2Seq_WaitMain
static BOOL func_ov137_021dcae8(Guru2CallWork *wk) {
    wk->seq = GURU2_SEQ_END;
    return FALSE;
}

// pokebw2's Guru2Seq_End
static BOOL func_ov137_021dcaf0(Guru2CallWork *wk) {
    func_ov137_021dc90c(wk->g2p);
    return TRUE;
}
