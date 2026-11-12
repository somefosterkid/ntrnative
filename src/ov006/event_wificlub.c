// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov008/event_wificlub.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/comm_tvt.h"
#include "app/pokemon_trade.h"
#include "app/ui/ui_scene.h"
#include "app/wifi_login.h"
#include "app/wifi_status.h"
#include "app/wificlub.h"
#include "battle/battle_proc.h"
#include "battle/battle_select.h"
#include "battle/btl_net.h"
#include "battle/btl_setup.h"
#include "battle/regulation.h"
#include "constants/sound.h"
#include "demo/shinka_demo.h"
#include "field/event_wificlub.h"
#include "field/field_event.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "save/wifi_list.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/ringtone_sys.h"

typedef struct {
    u8 battleMode;
    u16 nextState;
} WifiClubMode;

typedef struct {
    GameEvent *event;
    GameSystem *gsys;
    Field *field;
    void *self;
    u32 unk10;
    BOOL useTransitions;
    u32 unk18;
    GameProcManager *procManager;
    BattleSelectParam select;
    CommTvtParam tvt;
    WifiClubData *club;
    PokemonTradeParam trade;
    GameData *gameData;
    WifiList *wifiList;
    WifiLoginParam login;
    WifiLogoutParam logout;
    BtlSetup *btlSetup;
    BattlePlayers players;
    u8 unk130[4];
    GameRecords *records;
    BattleParam battle;
    PokeParty *party;
    u32 unk16C;
    u16 bgm;
    u8 battleMode;
} EventWifiClub;

void func_ov006_02154abc(EventWifiClub *wk);
GameEventReturnCode func_ov006_021544f0(GameEvent *event, u32 *state, void *data);
void func_ov006_02154b08(GameEvent *event, GameSystem *gsys, Field *field, BOOL useTransitions);

void func_ov006_021542c0(EventWifiClub *wk, u32 mode);
void func_ov006_0215444c(EventWifiClub *wk, GameData *gameData, u32 unused);
void func_ov006_021544bc(EventWifiClub *wk, GameData *gameData, u32 unused);
void func_ov006_021544d4(EventWifiClub *wk);

// pokebw2's sWifiClubModes
static WifiClubMode data_ov006_02154c40[16] = {
#include "asset/event_wificlub_data_ov006_02154c40.inc"
};

// pokebw2's EventWifiClub_SetupBattleSelect

void func_ov006_021542c0(EventWifiClub *wk, u32 mode) {
    GameData *gameData = GSYS_GetGameData(wk->gsys);
    u8 unk48 = wk->club->unk48;
    u32 rule;

    if (unk48) {
        func_0200aee8(wk->club->regulation, 13, TRUE);
    } else {
        func_0200aee8(wk->club->regulation, 13, FALSE);
    }

    switch (mode) {
    case 8:
        rule = 1;
        if (unk48) {
            rule = 3;
        }
        break;
    case 7:
        rule = 0;
        if (unk48) {
            rule = 2;
        }
        break;
    case 10:
        rule = 5;
        if (unk48) {
            rule = 7;
        }
        break;
    case 9:
        rule = 4;
        if (unk48) {
            rule = 6;
        }
        break;
    case 12:
        rule = 9;
        if (unk48) {
            rule = 11;
        }
        break;
    case 11:
        rule = 8;
        if (unk48) {
            rule = 10;
        }
        break;
    case 14:
        rule = 13;
        if (unk48) {
            rule = 15;
        }
        break;
    case 13:
        rule = 12;
        if (unk48) {
            rule = 14;
        }
        break;
    }
    wk->battle.rule = rule;
    wk->battle.unk10 = 0;

    switch (mode) {
    case 7:
    case 8:
        BtlSetup_SetNet1v1Single(wk->btlSetup, gameData, func_020399a8(), 1, HEAPID_GAMEEVENT);
        break;
    case 9:
    case 10:
        BtlSetup_SetNet1v1Double(wk->btlSetup, gameData, func_020399a8(), 1, HEAPID_GAMEEVENT);
        break;
    case 11:
    case 12:
        BtlSetup_SetNetTriple(wk->btlSetup, gameData, func_020399a8(), 1, HEAPID_GAMEEVENT);
        break;
    case 13:
    case 14:
        BtlSetup_SetNetRotation(wk->btlSetup, gameData, func_020399a8(), 1, HEAPID_GAMEEVENT);
        break;
    }

    func_02013980(wk->btlSetup, 1);
    func_02013160(wk->btlSetup, wk->club->regulation, HEAPID_GAMEEVENT);
    func_0201a2a8(wk->club->regulation, wk->club->parties[0]);
    func_0201a2a8(wk->club->regulation, wk->club->parties[1]);
    wk->btlSetup->records = GameData_GetRecords(GSYS_GetGameData(wk->gsys));
}

void func_ov006_0215444c(EventWifiClub *wk, GameData *gameData, u32 unused) {
    u32 netId = func_0203d1a4(func_020399a8());
    u32 otherNetId = 1 - netId;
    PlayerInfo *other = func_02012950(gameData, otherNetId);
    BattleSelectParam *select = &wk->select;

    select->regulation = wk->club->regulation;
    select->party = wk->club->parties[netId];
    select->otherName = GetPlayerName(other);
    select->otherGender = getTrainerGender(other);
    select->otherParty = wk->club->parties[otherNetId];
    select->gameData = gameData;
    select->unk18 = 0;
    PokeParty_Init(wk->party);
    select->unk1C = wk->party;
    select->parties[0] = wk->club->parties[0];
    select->parties[1] = wk->club->parties[1];
}

// pokebw2's EventWifiClub_SetBattleParty
void func_ov006_021544bc(EventWifiClub *wk, GameData *gameData, u32 unused) {
    func_02013128(wk->btlSetup, wk->party, 0);
}

// Resets the volume before logging in again
// pokebw2's EventWifiClub_ResetForLogin
void func_ov006_021544d4(EventWifiClub *wk) {
    GFL_SndPlayerSetVolumeEx(SND_VOLUME_MAX, SND_PLAYER_MASK_ALL);
    PokeVoice_SetMasterVolume(SND_VOLUME_MAX);
    wk->login.unk14 = 1;
}

extern u32 OVERLAY_99_ID[];
extern u32 OVERLAY_127_ID[];
extern u32 OVERLAY_134_ID[];
extern u32 OVERLAY_8_ID[];
extern u32 OVERLAY_119_ID[];
extern u32 OVERLAY_195_ID[];
extern u32 OVERLAY_172_ID[];
extern const GameProcFunctions data_ov115_021d6338;
extern const GameProcFunctions data_ov099_021c37e8;
extern const GameProcFunctions data_ov008_0215513c;
extern const GameProcFunctions data_ov119_021e75c0;
extern const GameProcFunctions data_ov195_02208c40;
extern const GameProcFunctions data_ov172_021c7240;
BOOL func_0203cdbc(void);
BOOL func_0203cd98(void);
BOOL func_0200fb24(void);
void func_0200f83c(void);
void func_02039c68(NetHandle *handle, u32 a1, u32 a2);
BOOL func_02039cb4(NetHandle *handle, u32 a1, u32 a2);
BOOL func_0203d210(void);
WifiList *func_0201283c(GameData *gameData);
BOOL func_02009a34(WifiList *wifiList);
void func_02029f6c(void);

GameEventReturnCode func_ov006_021544f0(GameEvent *event, u32 *state, void *data) {
    EventWifiClub *wk = data;
    GameSystem *gsys = wk->gsys;
    u32 seq = *state;

    switch (seq) {
    case 0:
        wk->bgm = GFL_SndBGMGetID();
        GFL_SndBGMFadeOut(6);
        (*state)++;
        break;
    case 1:
        if (!GameCommSys_BootCheck(GSYS_GetGameCommSystem(gsys))) {
            wk->unk10 = 0;
            GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, wk->field));
            (*state)++;
        }
        break;
    case 2:
        GFL_SndSetVolumeControlCallbacks();
        *state = 8;
        break;
    case 3:
        break;
    case 4:
        if (func_0203cdbc()) {
            (*state)++;
            func_ov006_02154abc(wk);
            PokeVoice_ResetMasterVolume();
            GFL_SndBGMPlay(wk->bgm, SND_CHANNEL_MASK_ALL);
            GFL_SndBGMFadeIn(60);
        }
        break;
    case 5:
        func_02029f6c();
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        (*state)++;
        break;
    case 6:
        if (wk->useTransitions) {
            GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, wk->field, 0, 0, 1, 0, 0));
        }
        (*state)++;
        break;
    case 7:
        return GAMEEVENT_DONE;
    case 8:
        MI_CpuFill8(&wk->login, 0, sizeof(WifiLoginParam));
        *state = 9;
        wk->login.unk14 = 0;
        break;
    case 9:
        wk->login.gameData = GSYS_GetGameData(gsys);
        wk->login.unk8 = 1;
        wk->login.unk4 = 0;
        wk->login.unkC = 10;
        wk->login.unk18 = 1;
        GFL_SndBGMPlay(SEQ_BGM_WIFI_ACCESS, SND_CHANNEL_MASK_ALL);
        wk->procManager = CreateGameProcManager(HEAPID_GAMEEVENT);
        QueueGameProc(wk->procManager, OVERLAY_ID(115), &data_ov115_021d6338, &wk->login);
        wk->club->mode = 0;
        *state = 10;
        break;
    case 10:
        if (GFL_ProcMgrUpdate(wk->procManager)) {
            GFL_NetErrShow(0);
            return GAMEEVENT_CONTINUE;
        }
        FreeGameProcManager(wk->procManager);
        if (wk->login.result == 0) {
            if (!func_02009a34(func_0201283c(wk->gameData))) {
                *state = 30;
            } else {
                *state = 11;
                GFL_SndBGMFadeOut(6);
            }
        } else {
            GFL_SndBGMFadeOut(6);
            *state = 25;
        }
        break;
    case 30:
        wk->logout.gameData = GSYS_GetGameData(gsys);
        wk->logout.unk8 = 1;
        wk->logout.unk4 = 0;
        wk->logout.unkC = 1;
        wk->logout.unk10 = 0;
        wk->logout.unk14 = 0;
        wk->logout.unk18 = 0;
        wk->procManager = CreateGameProcManager(HEAPID_GAMEEVENT);
        QueueGameProc(wk->procManager, OVERLAY_ID(115), &data_ov115_021d654c, &wk->logout);
        (*state)++;
        break;
    case 31:
        if (GFL_ProcMgrUpdate(wk->procManager)) {
            GFL_NetErrShow(0);
            return GAMEEVENT_CONTINUE;
        }
        FreeGameProcManager(wk->procManager);
        GFL_SndBGMFadeOut(6);
        *state = 4;
        break;
    case 11:
        GFL_OvlLoad(OVERLAY_ID(99));
        GFL_OvlLoad(OVERLAY_ID(67));
        func_ov099_021c2850(wk->club->buffer);
        if (wk->btlSetup != NULL) {
            BtlSetup_Free(wk->btlSetup);
            wk->btlSetup = NULL;
        }
        GSYS_QueueProcAsEvent(wk->event, OVERLAY_ID(127), &data_ov099_021c37e8, wk->club);
        (*state)++;
        break;
    case 12:
        *state = data_ov006_02154c40[wk->club->mode].nextState;
        if (wk->club->mode == 1) {
            wk->login.unk14 = 1;
        }
        wk->battleMode = data_ov006_02154c40[wk->club->mode].battleMode;
        GFL_OvlUnload(OVERLAY_ID(67));
        GFL_OvlUnload(OVERLAY_ID(99));
        break;
    case 13:
        func_ov006_0215444c(wk, GSYS_GetGameData(gsys), seq);
        wk->btlSetup = BtlSetup_Create(HEAPID_GAMEEVENT);
        func_ov006_021542c0(wk, wk->club->mode);
        GSYS_QueueProcAsEvent(wk->event, OVERLAY_ID(134), &data_ov134_021dcca4, &wk->select);
        (*state)++;
        break;
    case 14:
        if (wk->select.result == 1) {
            if (func_0203cd98()) {
                wk->club->unk49 = 0;
                *state = 11;
            } else {
                func_ov006_021544d4(wk);
                *state = 9;
            }
        } else {
            func_ov006_021544bc(wk, GSYS_GetGameData(gsys), seq);
            GFL_OvlLoad(OVERLAY_ID(93));
            func_0203a574(0x100, data_ov093_021f03c4, 9, 0);
            func_02039c68(func_020399a8(), 100, 10);
            (*state)++;
        }
        break;
    case 15:
        if (!func_0203cd98() || func_0200fb24()) {
            func_0200fbb0();
            func_0200f83c();
            func_ov006_021544d4(wk);
            *state = 9;
        } else if (func_02039cb4(func_020399a8(), 100, 10)) {
            (*state)++;
        }
        break;
    case 16: {
        int i;

        GFL_SndBGMPlay(SEQ_BGM_VS_TRAINER_WIFI, SND_CHANNEL_MASK_ALL);
        wk->players.unk44 = 1;
        if (func_0203d1a4(func_020399a8()) == 0) {
            for (i = 0; i < 2; i++) {
                wk->players.players[i].info = func_02012950(GSYS_GetGameData(gsys), i);
                wk->players.players[i].party = wk->club->parties[i];
            }
        } else {
            u8 order[2] = { 1, 0 };

            for (i = 0; i < 2; i++) {
                wk->players.players[order[i]].info = func_02012950(GSYS_GetGameData(gsys), i);
                wk->players.players[order[i]].party = wk->club->parties[i];
            }
        }
        wk->battle.gameData = GSYS_GetGameData(wk->gsys);
        wk->battle.setup = wk->btlSetup;
        wk->battle.players = &wk->players;
        wk->battle.unk14 = 1;
        wk->records = GameData_GetRecords(GSYS_GetGameData(wk->gsys));
        GFL_OvlUnload(OVERLAY_ID(93));
        GSYS_QueueProcAsEvent(wk->event, OVERLAY_ID(8), &data_ov008_0215513c, &wk->battle);
        (*state)++;
        break;
    }
    case 17:
        if (!func_0203cd98()) {
            func_ov006_021544d4(wk);
            *state = 9;
        } else {
            if (wk->players.result == 0) {
                func_02009bb8(func_0201283c(GSYS_GetGameData(wk->gsys)), wk->club->friendIndex - 1, 1, 0, 0);
            } else if (wk->players.result == 1) {
                func_02009bb8(func_0201283c(GSYS_GetGameData(wk->gsys)), wk->club->friendIndex - 1, 0, 1, 0);
            }
            func_0203a5e4(0x100);
            *state = 11;
        }
        break;
    case 18:
        wk->trade.next = 0;
        wk->trade.friendIndex = wk->club->friendIndex;
        (*state)++;
        break;
    case 19:
        wk->trade.gameData = GSYS_GetGameData(gsys);
        GSYS_QueueProcAsEvent(wk->event, OVERLAY_ID(119), &data_ov119_021e75c0, &wk->trade);
        (*state)++;
        break;
    case 20:
        switch (wk->trade.next) {
        case 1:
            *state = 21;
            break;
        case 2:
            wk->club->mode = 1;
            *state = 11;
            break;
        default:
            *state = 11;
            break;
        }
        if (!func_0203cd98()) {
            func_ov006_021544d4(wk);
            *state = 9;
        }
        break;
    case 21: {
        ShinkaDemoParam *evolution =
            GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(ShinkaDemoParam), FALSE, "event_wificlub.c", 587);

        evolution->gameData = GSYS_GetGameData(wk->gsys);
        evolution->party = wk->trade.party;
        evolution->species = wk->trade.evolveSpecies;
        evolution->partyIndex = 0;
        evolution->method = wk->trade.evolveMethod;
        evolution->unkC = 1;
        evolution->canCancel = FALSE;
        wk->trade.evolution = evolution;
        GSYS_QueueProcAsEvent(wk->event, OVERLAY_ID(195), &data_ov195_02208c40, evolution);
        *state = 22;
        break;
    }
    case 22:
        GFL_HeapFree(wk->trade.evolution);
        wk->trade.next = 1;
        *state = 19;
        if (!func_0203cd98()) {
            func_ov006_021544d4(wk);
            *state = 9;
        }
        break;
    case 23:
        wk->tvt.gameData = GSYS_GetGameData(gsys);
        wk->tvt.unk4 = 3;
        GSYS_QueueProcAsEvent(wk->event, OVERLAY_ID(172), &data_ov172_021c7240, &wk->tvt);
        (*state)++;
        break;
    case 24:
        if (!func_0203cd98()) {
            func_ov006_021544d4(wk);
            *state = 9;
        } else {
            *state = 11;
        }
        break;
    case 28:
        func_0203ceac(0);
        *state = 29;
        break;
    case 29:
        if (func_0203d210()) {
            *state = 4;
        }
        break;
    case 25:
    case 26:
    case 27:
        *state = 4;
        break;
    }
    return GAMEEVENT_CONTINUE;
}

// pokebw2's EventWifiClub_Free
void func_ov006_02154abc(EventWifiClub *wk) {
    if (wk->btlSetup != NULL) {
        BtlSetup_Free(wk->btlSetup);
        wk->btlSetup = NULL;
    }
    GFL_HeapFree(wk->club->parties[0]);
    GFL_HeapFree(wk->club->parties[1]);
    GFL_HeapFree(wk->club->regulation);
    GFL_HeapFree(wk->party);
    GFL_HeapFree(wk->club->buffer);
    GFL_HeapFree(wk->club);
}

Regulation *func_0200adf0(HeapID heapId);
void func_ov006_02154b08(GameEvent *event, GameSystem *gsys, Field *field, BOOL useTransitions) {
    EventWifiClub *wk;

    if (GameCommSys_BootCheck(GSYS_GetGameCommSystem(gsys))) {
        func_02026400(GSYS_GetGameCommSystem(gsys));
    }

    wk = GameEvent_GetData(event);
    wk->gsys = gsys;
    wk->field = field;
    wk->event = event;
    wk->useTransitions = useTransitions;
    wk->club = GFL_HeapAllocate(0x8004, sizeof(WifiClubData), TRUE, "event_wificlub.c", 688);
    wk->club->buffer = GFL_HeapAllocate(0x8004, 0x20, TRUE, "event_wificlub.c", 689);
    wk->club->gameData = GSYS_GetGameData(wk->gsys);
    wk->club->save = GameData_GetSaveControl(wk->club->gameData);
    wk->club->unk46 = 1;
    wk->club->unk49 = 0;
    wk->gameData = wk->club->gameData;
    wk->wifiList = func_0201283c(wk->gameData);
    wk->club->mode = 0;
    wk->party = PokeParty_Create(0x8004);
    wk->trade.party = wk->party;
    wk->self = wk;
    wk->bgm = GFL_SndBGMGetID();
    wk->club->parties[0] = PokeParty_Create(0x8001);
    wk->club->parties[1] = PokeParty_Create(0x8001);
    wk->club->regulation = func_0200adf0(0x8001);
    wk->club->unk20 = 1;
}


// pokebw2's EventWifiClub_Create
GameEvent *func_ov006_02154bf8(GameSystem *gsys, Field *field, BOOL useTransitions) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov006_021544f0, sizeof(EventWifiClub));

    func_ov006_02154b08(event, gsys, field, useTransitions);
    return event;
}

// Called through GameEvent_CreateOverlayDelegate, by the NetConnectWiFiClub script command
// pokebw2's EventWifiClub_CreateFromArgs
GameEvent *func_ov006_02154c20(GameSystem *gsys, void *data) {
    EventWifiClubArgs *args = data;

    return func_ov006_02154bf8(gsys, args->field, args->useTransitions);
}
