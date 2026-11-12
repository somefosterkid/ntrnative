// Black's code at ov010 0x0215a07c-0x0215a438. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "app/p_status.h"
#include "field/event_poke_status.h"
#include "gfl/heap.h"
#include "pml/poke_party.h"
#include "save/pokedex.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "field/field_event.h"
#include "app/pokelist.h"

// pokebw2's PokeSelectWork, the party list events' data
typedef struct {
    GameSystem *gsys;
    Field *field;
    PokeListParam *listParam;
    // The summary screen the list can show, or NULL
    PStatusParam *summaryParam;
    u16 *picked;
    u16 *index;
} PokeSelectWork;

GameEventReturnCode func_ov010_0215a214(GameEvent *event, u32 *state, void *data);
// pokebw2's PokeMoveReplaceWork, the move-forgetting summary screen's data
typedef struct {
    GameSystem *gsys;
    Field *field;
    PStatusParam *summaryParam;
    u16 *forgot;
    u16 *slot;
} PokeMoveReplaceWork;

GameEventReturnCode func_ov010_0215a370(GameEvent *event, u32 *state, void *data);

GameEvent *func_ov010_0215a07c(GameSystem *gsys, u16 *picked, u16 *index) {
    GameData *gameData;
    PokeParty *party;
    PokeListParam *listParam;
    GameEvent *event;
    PokeSelectWork *work;

    gameData = GSYS_GetGameData(gsys);
    party = GameData_GetParty(gameData);
    listParam = GFL_HeapAllocate(4, sizeof(PokeListParam), 1, __FILE__, 0x40);
    listParam->party = party;
    listParam->mode = 3;
    listParam->unk38 = 0;
    listParam->index = 0;
    event = GameEvent_Create(gsys, NULL, func_ov010_0215a214, sizeof(PokeSelectWork));
    work = GameEvent_GetData(event);
    work->gsys = gsys;
    work->field = GSYS_GetField(gsys);
    work->listParam = listParam;
    work->summaryParam = NULL;
    work->picked = picked;
    work->index = index;
    return event;
}

GameEvent *func_ov010_0215a0e8(GameSystem *gsys, u16 *picked, u16 *index, u8 learnMask, u16 move) {
    GameData *gameData;
    PokeParty *party;
    PokeListParam *listParam;
    GameEvent *event;
    PokeSelectWork *work;

    gameData = GSYS_GetGameData(gsys);
    party = GameData_GetParty(gameData);
    listParam = GFL_HeapAllocate(4, sizeof(PokeListParam), 1, __FILE__, 0x66);
    listParam->party = party;
    listParam->mode = 0x18;
    listParam->unk38 = 0;
    listParam->index = 0;
    listParam->learnMask = learnMask;
    listParam->move = move;
    event = GameEvent_Create(gsys, NULL, func_ov010_0215a214, sizeof(PokeSelectWork));
    work = GameEvent_GetData(event);
    work->gsys = gsys;
    work->field = GSYS_GetField(gsys);
    work->listParam = listParam;
    work->summaryParam = NULL;
    work->picked = picked;
    work->index = index;
    return event;
}

GameEvent *func_ov010_0215a168(GameSystem *gsys, u16 *picked, u16 *index) {
    GameData *gameData;
    PokeParty *party;
    PokeDexSave *pokedex;
    PokeListParam *listParam;
    PStatusParam *summaryParam;
    GameEvent *event;
    PokeSelectWork *work;

    gameData = GSYS_GetGameData(gsys);
    party = GameData_GetParty(gameData);
    pokedex = GameData_GetPokedex(gameData);
    listParam = GFL_HeapAllocate(4, sizeof(PokeListParam), 1, __FILE__, 0x90);
    listParam->party = party;
    listParam->mode = 0x19;
    listParam->unk38 = 0;
    listParam->index = 0;
    summaryParam = GFL_HeapAllocate(4, sizeof(PStatusParam), 1, __FILE__, 0x97);
    summaryParam->party = party;
    summaryParam->gameData = gameData;
    summaryParam->dataType = PSTATUS_DATA_PARTY;
    summaryParam->mode = PSTATUS_MODE_1;
    summaryParam->partyCount = PokeParty_GetPkmCount(party);
    summaryParam->page = PSTATUS_PAGE_INFO;
    summaryParam->isNationalDex = PokeDex_IsNationalObtained(pokedex);
    summaryParam->fromFieldMenu = FALSE;
    event = GameEvent_Create(gsys, NULL, func_ov010_0215a214, sizeof(PokeSelectWork));
    work = GameEvent_GetData(event);
    work->gsys = gsys;
    work->field = GSYS_GetField(gsys);
    work->listParam = listParam;
    work->summaryParam = summaryParam;
    work->picked = picked;
    work->index = index;
    return event;
}

GameEventReturnCode func_ov010_0215a214(GameEvent *event, u32 *state, void *data) {
    PokeSelectWork *work = data;
    GameSystem *gsys = work->gsys;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, work->field, 0, 0));
        *state = 1;
        break;
    case 1:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, work->field));
        *state = 2;
        break;
    case 2:
        GameEvent_ChainNext(event, EventPokeList_Create(gsys, work->field, work->listParam, work->summaryParam));
        *state = 3;
        break;
    case 3:
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        *state = 4;
        break;
    case 4:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, work->field, 0, 0, 1, 0, 0));
        *state = 5;
        break;
    case 5:
        if ((s32)work->listParam->index <= 5) {
            *work->index = work->listParam->index;
            *work->picked = TRUE;
        } else {
            *work->index = 0;
            *work->picked = FALSE;
        }
        GFL_HeapFree(work->listParam);
        if (work->summaryParam != NULL) {
            GFL_HeapFree(work->summaryParam);
        }
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

// Black allocates the summary screen's parameters at 0x98 bytes here, the party list's size (0x28 in
// func_ov010_0215a168)
GameEvent *func_ov010_0215a2e4(GameSystem *gsys, Field *field, u16 partyIndex, u16 *forgot, u16 *slot, s32 forgetHm, u16 move) {
    GameData *gameData;
    PokeParty *party;
    PStatusParam *summaryParam;
    GameEvent *event;
    PokeMoveReplaceWork *work;

    gameData = GSYS_GetGameData(gsys);
    party = GameData_GetParty(gameData);
    summaryParam = GFL_HeapAllocate(4, sizeof(PokeListParam), 1, __FILE__, 0x11d);
    summaryParam->party = party;
    summaryParam->dataType = PSTATUS_DATA_PARTY;
    summaryParam->partyCount = PokeParty_GetPkmCount(party);
    summaryParam->partyIndex = partyIndex;
    summaryParam->page = PSTATUS_PAGE_SKILL;
    summaryParam->move = move;
    if (forgetHm == TRUE) {
        summaryParam->mode = PSTATUS_MODE_FORGET_HM;
    } else {
        summaryParam->mode = PSTATUS_MODE_FORGET_MOVE;
    }
    summaryParam->fromFieldMenu = FALSE;
    summaryParam->gameData = gameData;
    event = GameEvent_Create(gsys, NULL, func_ov010_0215a370, sizeof(PokeMoveReplaceWork));
    work = GameEvent_GetData(event);
    work->gsys = gsys;
    work->field = field;
    work->summaryParam = summaryParam;
    work->forgot = forgot;
    work->slot = slot;
    return event;
}

GameEventReturnCode func_ov010_0215a370(GameEvent *event, u32 *state, void *data) {
    PokeMoveReplaceWork *work = data;
    GameSystem *gsys = work->gsys;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, work->field, 0, 0));
        *state = 1;
        break;
    case 1:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, work->field));
        *state = 2;
        break;
    case 2:
        GSYS_QueueProc(gsys, OVERLAY_PSTATUS, &data_ov131_021dc800, work->summaryParam);
        *state = 3;
        break;
    case 3:
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        *state = 4;
        break;
    case 4:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, work->field, 0, 0, 1, 0, 0));
        *state = 5;
        break;
    case 5:
        if (work->summaryParam->result == PSTATUS_RESULT_FORGET) {
            *work->slot = work->summaryParam->slot;
            *work->forgot = TRUE;
        } else {
            *work->slot = 0;
            *work->forgot = FALSE;
        }
        GFL_HeapFree(work->summaryParam);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

