// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/event_ircbattle.c, ported by tools/port_file.py.
#include "types.h"
#include "app/irc_compatible.h"
#include "app/ircbattlematch.h"
#include "app/pokemon_trade.h"
#include "battle/battle_proc.h"
#include "battle/btl_net.h"
#include "battle/btl_setup.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "demo/shinka_demo.h"
#include "field/event_irc.h"
#include "field/event_make.h"
#include "field/field_event.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "nitro/gx.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/wifi_list.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

extern u32 OVERLAY_174_ID[];
extern u32 OVERLAY_167_ID[];

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
extern const GameProcFunctions data_ov101_021b74ac;
extern const GameProcFunctions data_ov021_021d3fe8;
extern const NetCommand data_ov093_021f03c4[9];
extern const GameProcFunctions data_ov119_021e75d8;

void setPartyLv50(PokeParty *party) {
    int i;

    for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
        setLevel(PokeParty_GetPkm(party, i), 50);
    }
}

void battleBoxToLv50Party(BOOL useBattleBox, EventIRCWork *work) {
    int i;
    BattleBoxSave *battleBox;
    PartyPkm *pkm;
    PokeParty *party;

    battleBox = getBattleBox(work->save);
    if (!useBattleBox) {
        party = GameData_GetParty(GSYS_GetGameData(work->gsys));
        for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
            pkm = PokeParty_GetPkm(party, i);
            if (!PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL)) {
                PokeParty_AddPkm(work->party, pkm);
            }
        }
    } else {
        party = convertBoxedPokeSetToParty(battleBox, HEAPID_GAMEEVENT);
        PokeParty_Copy(party, work->party);
        GFL_HeapFree(party);
    }
    PokeParty_RecoverAll(work->party);
    setPartyLv50(work->party);
}

void TrimPartyTo3Members(PokeParty *party) {
    s32 i;
    s32 count = PokeParty_GetPkmCount(party);

    for (i = 3; i < count; i++) {
        PokeParty_RemovePkm(party, 3);
    }
}

void func_ov010_02155224(EventIRCWork *work) {
    s32 i;

    TrimPartyTo3Members(work->party);
    for (i = 0; i < 4; i++) {
        TrimPartyTo3Members(work->parties[i]);
    }
}

// Records the battle with each other player in the friend list
void func_ov010_02155244(EventIRCWork *work, GameData *gameData) {
    s32 partner = 0;
    s32 count;
    u32 place;
    u32 partnerPlace;
    s32 i;
    PlayerInfo *info;
    u32 friendIndex;

    if (work->btlSetup->fieldSituation.unk1a == 0) {
        partner = -1;
        count = 2;
    } else {
        place = work->ircMatch.order[func_0203d1a4(func_020399a8())];
        if (place < 2) {
            partnerPlace = 1 - place;
        } else {
            partnerPlace = 3 - (place - 2);
        }
        for (i = 0; i < 4; i++) {
            if (partnerPlace == work->ircMatch.order[i]) {
                partner = i;
                break;
            }
        }
        count = 4;
    }
    for (i = 0; i < count; i++) {
        info = func_02012950(gameData, i);
        if (i != func_0203d1a4(func_020399a8()) && info != NULL
            && func_02009d1c(func_0201283c(gameData), info, &friendIndex)) {
            if (i == partner) {
                func_02009b80(func_0201283c(gameData), friendIndex);
            } else if (work->players.result == 0) {
                func_02009bb8(func_0201283c(gameData), friendIndex, 1, 0, 0);
            } else if (work->players.result == 1) {
                func_02009bb8(func_0201283c(gameData), friendIndex, 0, 1, 0);
            } else {
                func_02009b80(func_0201283c(gameData), friendIndex);
            }
        }
    }
}

// Puts each player's party and info in their place in the battle
void func_ov010_02155328(EventIRCWork *work, GameSystem *gsys) {
    if (work->btlSetup->fieldSituation.unk1a) {
        if (work->ircMatch.order[func_0203d1a4(func_020399a8())] < 2) {
            s32 i = 0;

            for (; i < 4; i++) {
                work->players.players[work->ircMatch.order[i]].party = work->parties[i];
                work->players.players[work->ircMatch.order[i]].info = func_02012950(GSYS_GetGameData(gsys), i);
            }
        } else {
            s32 i = 0;
            u8 places[4] = { 2, 3, 0, 1 };

            for (; i < 4; i++) {
                work->players.players[places[work->ircMatch.order[i]]].party = work->parties[i];
                work->players.players[places[work->ircMatch.order[i]]].info = func_02012950(GSYS_GetGameData(gsys), i);
            }
        }
    } else if (func_0203d1a4(func_020399a8()) == 0) {
        s32 i = 0;

        for (; i < 2; i++) {
            work->players.players[work->ircMatch.order[i]].party = work->parties[i];
            work->players.players[work->ircMatch.order[i]].info = func_02012950(GSYS_GetGameData(gsys), i);
        }
    } else {
        s32 i = 0;
        u8 places[2] = { 1, 0 };

        for (; i < 2; i++) {
            work->players.players[places[work->ircMatch.order[i]]].party = work->parties[i];
            work->players.players[places[work->ircMatch.order[i]]].info = func_02012950(GSYS_GetGameData(gsys), i);
        }
    }
}

GameEventReturnCode EventIRC_Callback(GameEvent *event, u32 *state, void *data) {
    EventIRCWork *work = data;
    GameSystem *gsys = work->gsys;
    Field *field = GSYS_GetField(gsys);
    ShinkaDemoParam *evolution;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, 0, 0));
        (*state)++;
        break;
    case 1:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, field));
        *state = 2;
        break;
    case 2:
        GSYS_QueueProc(gsys, OVERLAY_ID(21), &data_ov021_021d3fe8, work);
        (*state)++;
        break;
    case 3:
        if (GSYS_GetProcMgrState(gsys)) {
            break;
        }
        if (work->mode == 8) {
            *state = 24;
        } else if (work->mode == 12) {
            *state = 21;
        } else {
            (*state)++;
        }
        break;
    case 4:
        (*state)++;
        break;
    case 5:
        battleBoxToLv50Party(work->useBattleBox, work);
        work->ircMatch.gameData = work->gameData;
        work->ircMatch.party = work->party;
        work->ircMatch.parties[0] = work->parties[0];
        work->ircMatch.parties[1] = work->parties[1];
        work->ircMatch.parties[2] = work->parties[2];
        work->ircMatch.parties[3] = work->parties[3];
        work->ircMatch.result = work->mode;
        GSYS_QueueProc(gsys, OVERLAY_IRCBATTLEMATCH, &data_ov100_021bb6a4, &work->ircMatch);
        (*state)++;
        break;
    case 6:
        if (GSYS_GetProcMgrState(gsys)) {
            break;
        }
        work->mode = work->ircMatch.result;
        switch (work->mode) {
        case 7:
            *state = 20;
            break;
        case 12:
            *state = 20;
            break;
        case 13:
            *state = 2;
            break;
        case 6:
            work->trade.next = 0;
            *state = 13;
            break;
        default:
            *state = 7;
            break;
        }
        break;
    case 7:
        GFL_OvlLoad(OVERLAY_BATTLE_MAIN);
        func_0203a574(0x100, data_ov093_021f03c4, 9, NULL);
        func_02039c68(func_020399a8(), 0x6e, 12);
        (*state)++;
        break;
    case 8:
        if (func_02039cb4(func_020399a8(), 0x6e, 12)) {
            (*state)++;
        }
        if (func_0200fb24()) {
            if (func_0200fbb0() == TRUE) {
                func_0200f83c();
            } else {
                GFL_NetErrAbort();
            }
            GFL_OvlUnload(OVERLAY_BATTLE_MAIN);
            *state = 20;
        }
        break;
    case 9:
        work->btlSetup = BtlSetup_Create(HEAPID_GAMEEVENT);
        work->records = GameData_GetRecords(work->gameData);
        work->players.unk4C = 0;
        switch (work->mode) {
        case 1:
            work->players.unk44 = 1;
            work->players.rule = 2;
            BtlSetup_SetNet1v1Single(work->btlSetup, work->gameData, func_020399a8(), 1, HEAPID_GAMEEVENT);
            break;
        case 2:
            work->players.unk44 = 1;
            work->players.rule = 6;
            BtlSetup_SetNet1v1Double(work->btlSetup, work->gameData, func_020399a8(), 1, HEAPID_GAMEEVENT);
            break;
        case 3:
            work->players.unk44 = 1;
            work->players.rule = 10;
            BtlSetup_SetNetTriple(work->btlSetup, work->gameData, func_020399a8(), 1, HEAPID_GAMEEVENT);
            break;
        case 4:
            work->players.unk44 = 1;
            work->players.rule = 14;
            BtlSetup_SetNetRotation(work->btlSetup, work->gameData, func_020399a8(), 1, HEAPID_GAMEEVENT);
            break;
        case 5: {
            u32 positions[4] = { 0, 2, 1, 3 };

            work->players.unk44 = 3;
            work->players.rule = 18;
            BtlSetup_SetNetMultiVsNet(work->btlSetup, work->gameData, func_020399a8(), 1,
                                      positions[work->ircMatch.order[func_0203d1a4(func_020399a8())]],
                                      HEAPID_GAMEEVENT);
            work->btlSetup->fieldSituation.unk1a = 1;
            work->btlSetup->battleStyle = 1;
            func_ov010_02155224(work);
            break;
        }
        }
        func_02013128(work->btlSetup, work->party, 0);
        func_02013980(work->btlSetup, 1);
        work->bgm = GFL_SndBGMGetID();
        func_02005d28();
        func_02039c68(func_020399a8(), 10, 12);
        (*state)++;
        break;
    case 10:
        if (func_0200fb24()) {
            BtlSetup_Free(work->btlSetup);
            work->btlSetup = NULL;
            if (func_0200fbb0() == TRUE) {
                func_0200f83c();
            } else {
                GFL_NetErrAbort();
            }
            GFL_OvlUnload(OVERLAY_BATTLE_MAIN);
            *state = 20;
        } else if (func_02039cb4(func_020399a8(), 10, 12)) {
            if (func_0203d378()) {
                GFL_SndBGMPlay(SEQ_BGM_VS_TRAINER_M, SND_CHANNEL_MASK_ALL);
            } else {
                GFL_SndBGMPlay(SEQ_BGM_VS_TRAINER_S, SND_CHANNEL_MASK_ALL);
            }
            (*state)++;
        }
        break;
    case 11: {
        EventMakeArgs args;

        func_ov010_02155328(work, gsys);
        GFL_OvlUnload(OVERLAY_BATTLE_MAIN);
        args.setup = work->btlSetup;
        args.players = &work->players;
        args.unk08 = 1;
        GameEvent_ChainNext(event, GameEvent_CreateOverlayDelegate(gsys, OVERLAY_BATTLE, eventMakeFunc, &args));
        (*state)++;
        break;
    }
    case 12:
        if (GSYS_GetProcMgrState(gsys)) {
            break;
        }
        func_ov010_02155244(work, GSYS_GetGameData(gsys));
        BtlSetup_Free(work->btlSetup);
        work->btlSetup = NULL;
        func_02005d28();
        func_ov010_02155a6c(work);
        *state = 17;
        break;
    case 13:
        work->trade.gameData = work->gameData;
        work->trade.party = work->party;
        GSYS_QueueProc(gsys, OVERLAY_POKEMONTRADE, &data_ov119_021e75d8, &work->trade);
        (*state)++;
        break;
    case 14:
        if (GSYS_GetProcMgrState(gsys)) {
            break;
        }
        if (work->trade.next == TRADE_NEXT_EVOLVE) {
            *state = 15;
        } else {
            *state = 17;
        }
        break;
    case 15:
        evolution = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(ShinkaDemoParam), FALSE, "event_ircbattle.c", 537);
        evolution->gameData = work->gameData;
        evolution->party = work->trade.party;
        evolution->species = work->trade.evolveSpecies;
        evolution->partyIndex = 0;
        evolution->method = work->trade.evolveMethod;
        evolution->unkC = 1;
        evolution->canCancel = FALSE;
        work->trade.evolution = evolution;
        GSYS_QueueProc(gsys, OVERLAY_SHINKA_DEMO, &data_ov195_02208c40, evolution);
        (*state)++;
        break;
    case 16:
        if (GSYS_GetProcMgrState(gsys)) {
            break;
        }
        GFL_HeapFree(work->trade.evolution);
        work->trade.next = TRADE_NEXT_EVOLVE;
        *state = 13;
        break;
    case 17:
        if (func_0203cd98() && !func_0200fb24()) {
            func_02039c68(func_020399a8(), 0xdc, 12);
        }
        (*state)++;
        break;
    case 18:
        if (func_0203cd98() && !func_0200fb24()) {
            if (func_02039cb4(func_020399a8(), 0xdc, 12)) {
                work->counter = 0;
                (*state)++;
            }
        } else {
            work->counter = 0;
            (*state)++;
        }
        break;
    case 19:
        if (func_0203cd98()) {
            if (++work->counter >= 60) {
                func_0203ceac(0);
                (*state)++;
            }
        } else {
            (*state)++;
        }
        break;
    case 20:
        if (!func_0203cd98()) {
            func_0200fba0();
            *state = 21;
        }
        break;
    case 21:
        func_ov010_02155a6c(work);
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        (*state)++;
        break;
    case 22:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, 0, 0, 1, 0, 0));
        (*state)++;
        break;
    case 23:
        func_0203d0d0();
        func_ov010_02155a4c(work);
        return GAMEEVENT_DONE;
    case 24:
        (*state)++;
        break;
    case 25:
        (*state)++;
        break;
    case 26:
        work->ov175Param = work->gsys;
        GSYS_QueueProc(gsys, OVERLAY_ID(101), &data_ov101_021b74ac, &work->ov175Param);
        (*state)++;
        break;
    case 27:
        if (GSYS_GetProcMgrState(gsys)) {
            break;
        }
        GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
        *state = 20;
        break;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *CallIRC(GameSystem *gsys, Field *field, GameEvent *event, BOOL create) {
    EventIRCWork *work;
    s32 i;

    if (create) {
        event = GameEvent_Create(gsys, NULL, EventIRC_Callback, sizeof(EventIRCWork));
    } else {
        GameEvent_Transplant(event, EventIRC_Callback, sizeof(EventIRCWork));
    }
    work = GameEvent_GetData(event);
    work->save = GameData_GetSaveControl(GSYS_GetGameData(gsys));
    work->gameData = GSYS_GetGameData(gsys);
    work->gsys = gsys;
    for (i = 0; i < 4; i++) {
        work->parties[i] = PokeParty_Create(HEAPID_GAMEEVENT);
    }
    work->party = PokeParty_Create(HEAPID_GAMEEVENT);
    return event;
}

void func_ov010_02155a4c(EventIRCWork *work) {
    s32 i;

    for (i = 0; i < 4; i++) {
        GFL_HeapFree(work->parties[i]);
    }
    GFL_HeapFree(work->party);
}

// Brings back the music from before the battle
void func_ov010_02155a6c(EventIRCWork *work) {
    if (work->bgm) {
        GFL_SndBGMPlay(work->bgm, SND_CHANNEL_MASK_ALL);
        GFL_SndBGMFadeIn(60);
        work->bgm = 0;
    }
}

// Accessors for the infrared menu of overlay 36
void func_ov010_02155a8c(EventIRCWork *work, u32 mode) {
    work->mode = mode;
}

GameSystem *func_ov010_02155a90(EventIRCWork *work) {
    return work->gsys;
}

SaveControl *func_ov010_02155a94(EventIRCWork *work) {
    return work->save;
}
