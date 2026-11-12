// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov213/battle_select.c, which is the same code in
// Black 2/White 2's overlay 213, except for the party list's setup. The names are Black's dsd names; pokebw2's
// guesses are in the comments.
#include "battle/battle_select.h"
#include "app/p_status.h"
#include "app/pokelist.h"
#include "battle/btl_setup.h"
#include "battle/regulation.h"
#include "gfl/dwc_rap.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_sync.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "pml/poke_party.h"
#include "save/pokedex.h"
#include "save/trainer_data_savedata.h"
#include "system/game_data.h"

// The net command that sends the team picked, after the party list's commands
#define BATTLE_SELECT_NET_CMD_BASE 0x2f00

typedef struct {
    u32 state;
    PokeListParam pokeList;
    PStatusParam status;
    // Frames counted toward the next second of the party list's time limit
    u8 frames;
    u32 lastVBlankCount;
    GameProcManager *procManager;
    BattleSelectParam param;
    HeapID heapId;
} BattleSelectWork;

static void *func_ov134_021dc860(int netId, void *work, int size);
static void func_ov134_021dc86c(int netId, int size, const void *data, void *work, NetHandle *handle);
static BOOL func_ov134_021dc888(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov134_021dc914(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov134_021dc93c(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov134_021dcb48(BattleSelectParam *param, BattleSelectWork *wk, PokeListParam *pokeList);
static void func_ov134_021dcc48(BattleSelectParam *param, BattleSelectWork *wk, PStatusParam *status);

// pokebw2's sBattleSelectNetCommands
static const NetCommand data_ov134_021dcc9c[] = {
    { func_ov134_021dc86c, func_ov134_021dc860 },
};

const GameProcFunctions data_ov134_021dcca4 = {
    func_ov134_021dc888,
    func_ov134_021dc93c,
    func_ov134_021dc914,
};

// pokebw2's BattleSelect_GetPartyBuffer
static void *func_ov134_021dc860(int netId, void *work, int size) {
    BattleSelectWork *wk = work;
    return wk->param.parties[netId];
}

// pokebw2's BattleSelect_RecvParty
static void func_ov134_021dc86c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    if (handle != func_020399a8()) {
        return;
    }
    if (netId == func_0203d1a4(func_020399a8())) {
        return;
    }
}

// pokebw2's BattleSelect_Init
static BOOL func_ov134_021dc888(GameProc *proc, u32 *state, void *param, void *work) {
    BattleSelectWork *wk = GFL_ProcInitSubsystem(proc, sizeof(BattleSelectWork), HEAPID_GAMEEVENT);
    MI_CpuFill8(wk, 0, sizeof(BattleSelectWork));
    wk->param = *(BattleSelectParam *)param;
    wk->heapId = HEAPID_GAMEEVENT;
    wk->procManager = CreateGameProcManager(wk->heapId);
    func_0203a574(BATTLE_SELECT_NET_CMD_BASE, data_ov134_021dcc9c, NELEMS(data_ov134_021dcc9c), wk);
    func_ov134_021dcb48(param, wk, &wk->pokeList);
    func_ov134_021dcc48(param, wk, &wk->status);
    wk->frames = 0;
    wk->lastVBlankCount = OS_GetVBlankCount();
    wk->state = 0;
    return TRUE;
}

// pokebw2's BattleSelect_Exit
static BOOL func_ov134_021dc914(GameProc *proc, u32 *state, void *param, void *work) {
    BattleSelectWork *wk = work;
    func_ov090_021b6108(&wk->pokeList);
    GFL_OvlUnload(OVERLAY_ID(90));
    FreeGameProcManager(wk->procManager);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's BattleSelect_Main
static BOOL func_ov134_021dc93c(GameProc *proc, u32 *state, void *param, void *work) {
    BattleSelectWork *wk = work;
    BattleSelectParam *select = param;
    u8 i;

    if (wk->pokeList.timerEnabled == TRUE && wk->pokeList.timeLeft != 0) {
        u32 vblankCount = OS_GetVBlankCount();
        wk->frames += (u8)(vblankCount - wk->lastVBlankCount);
        wk->lastVBlankCount = vblankCount;
        if (wk->frames > 60) {
            wk->pokeList.timeLeft--;
            wk->frames -= 60;
        }
    }
    func_ov090_021b6114(&wk->pokeList);

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
            select->result = 0;
            wk->state = 6;
        }
        break;
    case 6:
        for (i = 0; i < 6; i++) {
            if (wk->pokeList.picked[i] != 0) {
                PokeParty_AddPkm(select->unk1C, PokeParty_GetPkm(select->party, wk->pokeList.picked[i] - 1));
            }
        }
        wk->state = 7;
        break;
    case 7:
        if (func_0203d3f8(func_020399a8(), 0xff, BATTLE_SELECT_NET_CMD_BASE, PokeParty_GetSaveDataSize(),
                          select->unk1C, 0, FALSE, TRUE)) {
            func_02039c68(func_020399a8(), 20, 0x2f);
            wk->state = 8;
        }
        break;
    case 8:
        if (func_02039cb4(func_020399a8(), 20, 0x2f)) {
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
        if (func_0200fb24()) {
            wk->status.forceExit = TRUE;
        }
        if (!GFL_ProcMgrUpdate(wk->procManager)) {
            wk->state = 5;
        }
        break;
    case 5:
        GFL_OvlUnload(OVERLAY_PSTATUS);
        wk->pokeList.index = wk->status.partyIndex;
        wk->state = 0;
        break;
    }

    // Outside the screens, a lost connection ends the selection
    if (func_0203cd98() && wk->state != 1 && wk->state != 2 && wk->state != 4 && wk->state != 5) {
        if (func_ov009_02157170(1, 1)) {
            select->result = 1;
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's BattleSelect_SetupPokeList. Black sets up the party list's parameters here, where Black 2 calls
// PokeListParam_Setup
static void func_ov134_021dcb48(BattleSelectParam *param, BattleSelectWork *wk, PokeListParam *pokeList) {
    Regulation *regulation = param->regulation;
    SaveControl *save = GameData_GetSaveControl(param->gameData);
    u8 i;

    pokeList->party = param->party;
    func_0201a2a8(regulation, pokeList->party);
    pokeList->bag = NULL;
    pokeList->unk08 = NULL;
    pokeList->trainerData = getTrainerDataBlkAddress(save);
    pokeList->unk10 = NULL;
    pokeList->zoneId = 0;
    pokeList->mode = 0x1a;
    pokeList->regulation = regulation;
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
    pokeList->unk54 = 0;
    pokeList->learnIndex = 0;
    pokeList->unk58 = 0;
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
    pokeList->battleMsg = 0;
    GFL_OvlLoad(OVERLAY_ID(90));
    func_ov090_021b6100(pokeList);
}

// pokebw2's BattleSelect_SetupStatus
static void func_ov134_021dcc48(BattleSelectParam *param, BattleSelectWork *wk, PStatusParam *status) {
    SaveControl *save = GameData_GetSaveControl(param->gameData);
    PokeDexSave *pokedex;

    GetGameDataPlayerInfo(param->gameData);
    pokedex = GameData_GetPokedex(param->gameData);
    status->party = param->party;
    status->gameData = param->gameData;
    status->trainerData = getTrainerDataBlkAddress(save);
    status->dataType = PSTATUS_DATA_PARTY;
    status->mode = PSTATUS_MODE_1;
    status->partyCount = PokeParty_GetPkmCount(param->party);
    status->partyIndex = 0;
    status->page = PSTATUS_PAGE_INFO;
    status->slot = 0;
    status->result = 0;
    status->move = 0;
    status->isNationalDex = PokeDex_IsNationalObtained(pokedex);
    status->fromFieldMenu = FALSE;
    status->forceExit = FALSE;
}
