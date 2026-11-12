// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov263/wifibattlematch_subproc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/wifibattlematch_subproc.h"
#include "types.h"
#include "app/livebattlematch_irc.h"
#include "app/p_status.h"
#include "app/pokelist.h"
#include "app/wifibattlematch_net.h"
#include "battle/regulation.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/net_sync.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "pml/poke_party.h"
#include "save/pokedex.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "gfl/dwc_rap.h"
#include "save/trainer_data_savedata.h"

extern u32 OVERLAY_67_ID[];
extern u32 OVERLAY_90_ID[];
extern u32 OVERLAY_114_ID[];
extern u32 OVERLAY_175_ID[];
extern u32 OVERLAY_176_ID[];

#define HEAPID_WIFIBATTLEMATCH_SUBPROC 0x58

// GFLNetInitData.bNetType of an infrared connection
#define NET_TYPE_IRC 3

typedef struct {
    u32 state;
    PokeParty *party;
    PokeListParam pokeList;
    PStatusParam status;
    // Frames counted toward the next second of the party list's time limit
    u8 frames;
    u32 lastVBlankCount;
    GameProcManager *procManager;
    WifiBattleMatchListParam param;
    HeapID heapId;
} WifiBattleMatchListWork;

typedef struct {
    // A WifiBattleMatchNet or a LiveBattleMatchIrc, by the mode
    void *link;
} WifiBattleMatchExchangeWork;

static BOOL func_ov178_021e5440(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov178_021e54cc(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov178_021e5524(GameProc *proc, u32 *state, void *param, void *work);
void func_ov178_021e56f4(WifiBattleMatchListParam *param, WifiBattleMatchListWork *wk, PokeListParam *pokeList);
static void func_ov178_021e57e0(WifiBattleMatchListParam *param, WifiBattleMatchListWork *wk,
                                            PStatusParam *status);
static BOOL func_ov178_021e5838(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov178_021e5888(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov178_021e58bc(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov178_021e58f0(WifiBattleMatchExchangeWork *wk, WifiBattleMatchExchangeParam *param,
                                             HeapID heapId);
static BOOL func_ov178_021e58f4(WifiBattleMatchExchangeWork *wk, WifiBattleMatchExchangeParam *param,
                                             u32 *state);
static void func_ov178_021e5a44(WifiBattleMatchExchangeWork *wk, WifiBattleMatchExchangeParam *param);
static void func_ov178_021e5a70(WifiBattleMatchExchangeWork *wk, WifiBattleMatchExchangeParam *param,
                                            HeapID heapId);
static BOOL func_ov178_021e5a90(WifiBattleMatchExchangeWork *wk, WifiBattleMatchExchangeParam *param,
                                            u32 *state);
static void func_ov178_021e5b1c(WifiBattleMatchExchangeWork *wk, WifiBattleMatchExchangeParam *param);

// pokebw2's WIFIBATTLEMATCH_LIST_PROC_FUNCTIONS
const GameProcFunctions data_ov178_021e5b30 = {
    func_ov178_021e5440,
    func_ov178_021e5524,
    func_ov178_021e54cc,
};

// pokebw2's WIFIBATTLEMATCH_EXCHANGE_PROC_FUNCTIONS
const GameProcFunctions data_ov178_021e5b3c = {
    func_ov178_021e5838,
    func_ov178_021e58bc,
    func_ov178_021e5888,
};

// pokebw2's WifiBattleMatchList_Init
static BOOL func_ov178_021e5440(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchListParam *list = param;
    WifiBattleMatchListWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_WIFIBATTLEMATCH_SUBPROC, 0x30000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(WifiBattleMatchListWork), HEAPID_WIFIBATTLEMATCH_SUBPROC);
    MI_CpuFill8(wk, 0, sizeof(WifiBattleMatchListWork));
    wk->param = *list;
    wk->heapId = HEAPID_WIFIBATTLEMATCH_SUBPROC;
    wk->procManager = CreateGameProcManager(wk->heapId);
    wk->party = list->party;
    func_ov178_021e56f4(list, wk, &wk->pokeList);
    func_ov178_021e57e0(list, wk, &wk->status);
    wk->frames = 0;
    wk->lastVBlankCount = OS_GetVBlankCount();
    wk->state = 0;
    list->result = 0;
    return TRUE;
}

// pokebw2's WifiBattleMatchList_Exit
static BOOL func_ov178_021e54cc(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchListParam *list = param;
    WifiBattleMatchListWork *wk = work;
    u8 i;

    for (i = 0; i < 6; i++) {
        if (wk->pokeList.picked[i] != 0) {
            PokeParty_AddPkm(list->selected, PokeParty_GetPkm(list->party, wk->pokeList.picked[i] - 1));
        }
    }
    func_ov090_021b6108(&wk->pokeList);
    GFL_OvlUnload(OVERLAY_ID(90));
    FreeGameProcManager(wk->procManager);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_WIFIBATTLEMATCH_SUBPROC);
    return TRUE;
}

static BOOL func_ov178_021e5524(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchListParam *list = param;
    WifiBattleMatchListWork *wk = work;
    u32 vblankCount;

    if (wk->pokeList.timerEnabled == TRUE && wk->pokeList.timeLeft != 0) {
        vblankCount = OS_GetVBlankCount();
        wk->frames += (u8)(vblankCount - wk->lastVBlankCount);
        wk->lastVBlankCount = vblankCount;
        if (wk->frames > 60) {
            wk->pokeList.timeLeft--;
            wk->frames -= 60;
        }
    }

    if (func_0203cd98()) {
        if (func_0203d798()->bNetType == NET_TYPE_IRC) {
            if (func_0200fb24()) {
                func_0200fb38();
                list->result = 2;
                wk->pokeList.unk62[1] = TRUE;
                wk->status.forceExit = TRUE;
            }
        } else {
            switch (func_ov009_02157170(1, 1)) {
            case 0:
            case 3:
                break;
            case 4:
                list->result = 3;
                wk->pokeList.unk62[1] = TRUE;
                wk->status.forceExit = TRUE;
                break;
            case 1:
            case 2:
            default:
                list->result = 1;
                wk->pokeList.unk62[1] = TRUE;
                wk->status.forceExit = TRUE;
                break;
            }
        }
    }

    switch (wk->state) {
    case 0:
        GFL_OvlLoad(OVERLAY_ID(91));
        QueueGameProc(wk->procManager, OVERLAY_NONE, &data_ov091_021bed00, &wk->pokeList);
        wk->state = 1;
        break;
    case 1:
        if (!GFL_ProcMgrUpdate(wk->procManager)) {
            wk->state = 2;
        }
        break;
    case 2:
        GFL_OvlUnload(OVERLAY_ID(91));
        if (wk->pokeList.result == 1) {
            wk->status.partyIndex = wk->pokeList.index;
            wk->state = 3;
        } else {
            return TRUE;
        }
        break;
    case 3:
        GFL_OvlLoad(OVERLAY_PSTATUS);
        QueueGameProc(wk->procManager, OVERLAY_NONE, &data_ov131_021dc800, &wk->status);
        wk->state = 4;
        break;
    case 4:
        if (wk->pokeList.timerEnabled == TRUE && wk->pokeList.timeLeft == 0) {
            wk->status.forceExit = TRUE;
        }
        if (!GFL_ProcMgrUpdate(wk->procManager)) {
            wk->state = 5;
        }
        break;
    case 5:
        GFL_OvlUnload(OVERLAY_PSTATUS);
        if (wk->pokeList.unk62[1] == 0) {
            wk->pokeList.index = wk->status.partyIndex;
            wk->state = 0;
        } else {
            return TRUE;
        }
        break;
    }

    if (wk->pokeList.unk62[1] == 0) {
        func_ov090_021b6114(&wk->pokeList);
    }
    return FALSE;
}


void func_ov178_021e56f4(WifiBattleMatchListParam *param, WifiBattleMatchListWork *wk,
                                              PokeListParam *pokeList) {
    Regulation *regulation = param->regulation;
    SaveControl *save;
    u8 i;

    save = GameData_GetSaveControl(param->gameData);
    pokeList->party = wk->party;
    pokeList->bag = NULL;
    pokeList->unk08 = NULL;
    pokeList->trainerData = getTrainerDataBlkAddress(save);
    pokeList->unk10 = NULL;
    pokeList->regulation = regulation;
    pokeList->mode = 0x1a;
    pokeList->zoneId = 0;
    switch (regulation->unkBA) {
    case 0:
        pokeList->unk38 = 0;
        pokeList->partnerCount = 1;
        break;
    case 1:
        pokeList->unk38 = 1;
        pokeList->partnerCount = 1;
        break;
    case 2:
        pokeList->unk38 = 1;
        pokeList->partnerCount = 1;
        break;
    case 3:
        pokeList->unk38 = 1;
        pokeList->partnerCount = 1;
        break;
    case 4:
        pokeList->unk38 = 2;
        pokeList->partnerCount = 2;
        break;
    }
    pokeList->item = 0;
    pokeList->move = 0;
    pokeList->moveSlot = 0;
    pokeList->index = 0;
    pokeList->result = 0;
    for (i = 0; i < 6; i++) {
        pokeList->picked[i] = 0;
    }
    pokeList->learnIndex = 0;
    pokeList->unk58 = 0;
    pokeList->unk54 = 0;
    pokeList->partners[1].party = param->otherParty;
    pokeList->partners[1].name = param->otherName;
    pokeList->partners[1].gender = param->otherGender;
    pokeList->showPartners = regulation->showPartners;
    if (regulation->timeLimit != 0) {
        pokeList->timerEnabled = TRUE;
        pokeList->timeLeft = regulation->timeLimit;
    } else {
        pokeList->timerEnabled = FALSE;
        pokeList->timeLeft = 0;
    }
    GFL_OvlLoad(OVERLAY_ID(90));
    func_ov090_021b6100(pokeList);
}


static void func_ov178_021e57e0(WifiBattleMatchListParam *param, WifiBattleMatchListWork *wk,
                                            PStatusParam *status) {
    SaveControl *save = GameData_GetSaveControl(param->gameData);
    PokeDexSave *pokedex;

    GetGameDataPlayerInfo(param->gameData);
    pokedex = GameData_GetPokedex(param->gameData);
    status->party = wk->party;
    status->gameData = param->gameData;
    status->trainerData = getTrainerDataBlkAddress(save);
    status->dataType = PSTATUS_DATA_PARTY;
    status->mode = PSTATUS_MODE_1;
    status->partyCount = PokeParty_GetPkmCount(wk->party);
    status->partyIndex = 0;
    status->page = PSTATUS_PAGE_INFO;
    status->slot = 0;
    status->result = 0;
    status->move = 0;
    status->isNationalDex = PokeDex_IsNationalObtained(pokedex);
    status->fromFieldMenu = FALSE;
    status->forceExit = FALSE;
}

// pokebw2's WifiBattleMatchExchange_Init
static BOOL func_ov178_021e5838(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchExchangeParam *exchange = param;
    WifiBattleMatchExchangeWork *wk;
    HeapID heapId = HEAPID_WIFIBATTLEMATCH_SUBPROC;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_WIFIBATTLEMATCH_SUBPROC, 0x10000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(WifiBattleMatchExchangeWork), HEAPID_WIFIBATTLEMATCH_SUBPROC);
    MI_CpuFill8(wk, 0, sizeof(WifiBattleMatchExchangeWork));
    exchange->result = 0;
    switch (exchange->mode) {
    case WIFIBATTLEMATCH_EXCHANGE_WIFI:
        func_ov178_021e58f0(wk, exchange, heapId);
        break;
    case WIFIBATTLEMATCH_EXCHANGE_IRC:
        func_ov178_021e5a70(wk, exchange, heapId);
        break;
    }
    return TRUE;
}

// pokebw2's WifiBattleMatchExchange_Exit
static BOOL func_ov178_021e5888(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchExchangeParam *exchange = param;

    switch (exchange->mode) {
    case WIFIBATTLEMATCH_EXCHANGE_WIFI:
        func_ov178_021e5a44(work, exchange);
        break;
    case WIFIBATTLEMATCH_EXCHANGE_IRC:
        func_ov178_021e5b1c(work, exchange);
        break;
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_WIFIBATTLEMATCH_SUBPROC);
    return TRUE;
}

// pokebw2's WifiBattleMatchExchange_Main
static BOOL func_ov178_021e58bc(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchExchangeParam *exchange = param;
    BOOL done;

    switch (exchange->mode) {
    case WIFIBATTLEMATCH_EXCHANGE_WIFI:
        done = func_ov178_021e58f4(work, exchange, state);
        break;
    case WIFIBATTLEMATCH_EXCHANGE_IRC:
        done = func_ov178_021e5a90(work, exchange, state);
        break;
    }
    if (done) {
        return TRUE;
    }
    return FALSE;
}

static void func_ov178_021e58f0(WifiBattleMatchExchangeWork *wk, WifiBattleMatchExchangeParam *param,
                                             HeapID heapId) {
}

static BOOL func_ov178_021e58f4(WifiBattleMatchExchangeWork *wk, WifiBattleMatchExchangeParam *param,
                                             u32 *state) {
    if (func_0203cd98()) {
        switch (func_ov009_02157170(1, 1)) {
        case 0:
            break;
        case 1:
        case 4:
            param->result = 3;
            return TRUE;
        case 2:
        case 3:
        default:
            param->result = 1;
            return TRUE;
        }
    }

    switch (*state) {
    case 0:
        func_02039c68(func_020399a8(), 0x10, 0x24);
        (*state)++;
        break;
    case 1:
        if (func_02039cb4(func_020399a8(), 0x10, 0x24)) {
            (*state)++;
        }
        break;
    case 2:
        GFL_OvlLoad(OVERLAY_ID(67));
        GFL_OvlLoad(OVERLAY_ID(114));
        GFL_OvlLoad(OVERLAY_ID(175));
        wk->link = func_ov175_021d9dc8(param->unk14, *param->gameData, 0, HEAPID_WIFIBATTLEMATCH_SUBPROC);
        (*state)++;
        break;
    case 3:
        func_02039c68(func_020399a8(), 0x11, 0x24);
        (*state)++;
        break;
    case 4:
        if (func_02039cb4(func_020399a8(), 0x11, 0x24)) {
            (*state)++;
        }
        break;
    case 5:
        if (func_ov175_021dc200(wk->link, param->party)) {
            (*state)++;
        }
        break;
    case 6:
        if (func_ov175_021dc240(wk->link, param->otherParty)) {
            (*state)++;
        }
        break;
    case 7:
        func_02039c68(func_020399a8(), 0x12, 0x24);
        (*state)++;
        break;
    case 8:
        if (func_02039cb4(func_020399a8(), 0x12, 0x24)) {
            (*state)++;
        }
        break;
    case 9:
        func_ov175_021d9ea0(wk->link);
        wk->link = NULL;
        GFL_OvlUnload(OVERLAY_ID(175));
        GFL_OvlUnload(OVERLAY_ID(114));
        GFL_OvlUnload(OVERLAY_ID(67));
        (*state)++;
        break;
    case 10:
        func_02039c68(func_020399a8(), 0x13, 0x24);
        (*state)++;
        break;
    case 11:
        if (func_02039cb4(func_020399a8(), 0x13, 0x24)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's WifiBattleMatchExchange_ExitWifi
static void func_ov178_021e5a44(WifiBattleMatchExchangeWork *wk, WifiBattleMatchExchangeParam *param) {
    if (wk->link != NULL) {
        func_ov175_021d9ea0(wk->link);
        GFL_OvlUnload(OVERLAY_ID(175));
        GFL_OvlUnload(OVERLAY_ID(114));
        GFL_OvlUnload(OVERLAY_ID(67));
    }
}

static void func_ov178_021e5a70(WifiBattleMatchExchangeWork *wk, WifiBattleMatchExchangeParam *param,
                                            HeapID heapId) {
    GFL_OvlLoad(OVERLAY_ID(176));
    wk->link = func_ov176_0217e00c(*param->gameData, heapId);
}

static BOOL func_ov178_021e5a90(WifiBattleMatchExchangeWork *wk, WifiBattleMatchExchangeParam *param,
                                            u32 *state) {
    if (func_0200fb24()) {
        func_0200fb38();
        param->result = 2;
        return TRUE;
    }

    switch (*state) {
    case 0:
        (*state)++;
        break;
    case 1:
        func_02039c68(func_020399a8(), 0x11, 0x1f);
        (*state)++;
        break;
    case 2:
        if (func_02039cb4(func_020399a8(), 0x11, 0x1f)) {
            (*state)++;
        }
        break;
    case 3:
        if (func_ov176_0217e258(wk->link, param->party)) {
            (*state)++;
        }
        break;
    case 4:
        if (func_ov176_0217e298(wk->link, param->otherParty)) {
            (*state)++;
        }
        break;
    case 5:
        return TRUE;
    }
    func_ov176_0217e074(wk->link);
    return FALSE;
}

// pokebw2's WifiBattleMatchExchange_ExitIrc
static void func_ov178_021e5b1c(WifiBattleMatchExchangeWork *wk, WifiBattleMatchExchangeParam *param) {
    func_ov176_0217e058(wk->link);
    GFL_OvlUnload(OVERLAY_ID(176));
}
