// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov330/event_pdc_return.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// Named after the string event_pdc_return.c the overlay embeds
#include "types.h"
#include "app/event_pdc_return.h"
#include "app/name_entry.h"
#include "app/zukan_toroku.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/text_banks.h"
#include "field/player_state.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/str.h"
#include "pml/met_data.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/event_work.h"
#include "save/player_info.h"
#include "save/pokedex.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "field/field_actor.h"

#define HEAPID_PDC_RETURN 0x8b

// Set once the player knows whose PC the boxes are
#define FLAG_PDC_RETURN_BOX_MESSAGE 0x96d

typedef struct {
    NameEntryParam *nameEntry;
    // The player's name, then the nickname
    StrBuf *name;
    ZukanTorokuParam zukan;
    // Where the Pokémon goes when the party is full
    StrBuf *boxMessage;
    u32 box;
    u16 heapId;
    GameProcManager *procManager;
} EventPdcReturnWork;

static BOOL func_ov227_021b95d0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov227_021b9610(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov227_021b963c(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's EVENT_PDC_RETURN_PROC_FUNCTIONS
const GameProcFunctions data_ov227_021b98a4 = {
    func_ov227_021b95d0,
    func_ov227_021b963c,
    func_ov227_021b9610,
};

// pokebw2's EventPdcReturn_CreateParam
EventPdcReturnParam *func_ov227_021b95a0(GameData *gameData, u32 result, PartyPkm *pkm, HeapID heapId) {
    EventPdcReturnParam *param = GFL_HeapAllocate(heapId, sizeof(EventPdcReturnParam), FALSE, "event_pdc_return.c", 94);

    param->gameData = gameData;
    param->result = result;
    param->pkm = pkm;
    return param;
}

// pokebw2's EventPdcReturn_FreeParam
void func_ov227_021b95c8(EventPdcReturnParam *param) {
    GFL_HeapFree(param);
}

// pokebw2's EventPdcReturn_Init
static BOOL func_ov227_021b95d0(GameProc *proc, u32 *state, void *param, void *work) {
    EventPdcReturnWork *wk;

    GFL_HeapCreateChild(1, HEAPID_PDC_RETURN, 0x1000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(EventPdcReturnWork), HEAPID_PDC_RETURN);
    wk->name = GFL_StrBufCreate(32, HEAPID_PDC_RETURN);
    wk->nameEntry = NULL;
    wk->boxMessage = NULL;
    wk->box = 0;
    wk->heapId = HEAPID_PDC_RETURN;
    wk->procManager = CreateGameProcManager(wk->heapId);
    return TRUE;
}

// pokebw2's EventPdcReturn_Exit
static BOOL func_ov227_021b9610(GameProc *proc, u32 *state, void *param, void *work) {
    EventPdcReturnWork *wk = work;

    FreeGameProcManager(wk->procManager);
    if (wk->boxMessage != NULL) {
        GFL_StrBufFree(wk->boxMessage);
    }
    GFL_StrBufFree(wk->name);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_PDC_RETURN);
    return TRUE;
}

BOOL func_ov227_021b963c(GameProc *proc, u32 *state, void *param, void *work) {
    EventPdcReturnWork *wk = work;
    EventPdcReturnParam *prm = param;
    BOOL procRunning;
    PokeParty *party;
    PlayerInfo *playerInfo;
    PokeDexSave *pokedex;
    BoxSaveAccessor *boxes;
    GameRecords *records;
    BOOL nationalDex;
    MsgData *msgData;
    BOOL firstCatch;
    BOOL nickname;
    int box;
    int slot;

    procRunning = GFL_ProcMgrUpdate(wk->procManager);
    if (procRunning == TRUE) {
        return FALSE;
    }
    switch (*state) {
    case 0:
        party = GameData_GetParty(prm->gameData);
        playerInfo = GetGameDataPlayerInfo(prm->gameData);
        pokedex = GameData_GetPokedex(prm->gameData);
        boxes = GameData_GetBoxSaveAccessor(prm->gameData);
        if (prm->result == PDC_RESULT_CAUGHT) {
            records = GameData_GetRecords(prm->gameData);
            RecordAddOne(records, 7);
            RecordAddOne(records, 0x54);
            textCopy(playerInfo->name, wk->name);
            PokeParty_SetParam(prm->pkm, PKM_PARAM_OT_NAME, (uintptr_t)wk->name);
            if (PokeParty_GetPkmCount(party) >= PokeParty_GetCapacity(party)) {
                box = BoxSaveAccessor_GetLastOpenedBox(boxes);
                slot = 0;
                BoxSaveAccessor_GetNextFreeBoxSlot(boxes, &box, &slot);
                wk->box = box;
                msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_SERVER_FLOW_TITLE, HEAPID_PDC_RETURN);
                wk->boxMessage = GFL_MsgDataLoadStrbufNew(msgData,
                    EventWork_FlagGet(GameData_GetEventWork(prm->gameData), FLAG_PDC_RETURN_BOX_MESSAGE) ? 0xb1 : 0xb0);
                GFL_MsgDataFree(msgData);
            }
            PokeParty_SetupMetData(prm->pkm, 0, playerInfo,
                                   ZoneData_GetPlaceNameID(PlayerState_GetZoneID(GameData_GetPlayerState(prm->gameData))),
                                   wk->heapId);
            firstCatch = FALSE;
            nationalDex = PokeDex_IsNationalObtained(pokedex);
            if (!PokeDex_IsCaught(pokedex, PokeParty_GetParam(prm->pkm, PKM_PARAM_SPECIES, NULL))) {
                firstCatch = TRUE;
            }
            PokeDex_RegistPkm(pokedex, prm->pkm);
            addPkmToDex(pokedex, prm->pkm);
            wk->zukan.pkm = prm->pkm;
            wk->zukan.nationalDex = nationalDex;
            wk->zukan.boxMessage = wk->boxMessage;
            wk->zukan.boxes = boxes;
            wk->zukan.box = wk->box;
            wk->zukan.gameData = prm->gameData;
            if (firstCatch) {
                wk->zukan.mode = 0;
            } else {
                wk->zukan.mode = 1;
            }
            QueueGameProc(wk->procManager, OVERLAY_ID(207), &data_ov207_0220bab0, &wk->zukan);
            (*state)++;
        } else {
            if (prm->result == PDC_RESULT_SEEN) {
                PokeDex_RegistPkm(pokedex, prm->pkm);
            }
            *state = 4;
        }
        break;
    case 1:
        if (procRunning != TRUE) {
            boxes = GameData_GetBoxSaveAccessor(prm->gameData);
            nickname = FALSE;
            if (wk->zukan.nickname == TRUE) {
                nickname = TRUE;
            }
            if (nickname) {
                wk->nameEntry = pokemonNameEntry(wk->heapId, prm->pkm, 10, NULL, wk->boxMessage, boxes, wk->box,
                                                 getTrainerGameInfoAddress(GameData_GetSaveControl(prm->gameData)));
                QueueGameProc(wk->procManager, OVERLAY_ID(194), &data_ov194_021f5580, wk->nameEntry);
                (*state)++;
            } else {
                *state = 3;
            }
        } else {
            return FALSE;
        }
        break;
    case 2:
        if (procRunning != TRUE) {
            if (func_ov010_0216c0e4(wk->nameEntry) == FALSE) {
                func_ov010_0216c0d4(wk->nameEntry, wk->name);
                PokeParty_SetParam(prm->pkm, PKM_PARAM_NICKNAME, (uintptr_t)wk->name);
                RecordAddOne(GameData_GetRecords(prm->gameData), 0x1e);
            }
            func_ov010_0216c0c0(wk->nameEntry);
            wk->nameEntry = NULL;
            (*state)++;
        } else {
            return FALSE;
        }
        break;
    case 3:
        party = GameData_GetParty(prm->gameData);
        if (wk->boxMessage == NULL) {
            PokeParty_AddPkm(party, prm->pkm);
        } else {
            BoxSaveAccessor_InsertPkm(GameData_GetBoxSaveAccessor(prm->gameData), func_020185f0(prm->pkm));
        }
        (*state)++;
        break;
    case 4:
    default:
        return TRUE;
    }
    return FALSE;
}


