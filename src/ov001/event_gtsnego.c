// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov001/event_gtsnego.c, which is the same code
// in Black 2/White 2's overlay 1, with Black's overlays. The names are Black's dsd names and swan's; pokebw2's guesses
// are in the comments.
#include "types.h"
#include "app/gtsnego.h"
#include "app/pokemon_trade.h"
#include "app/wifi_login.h"
#include "demo/shinka_demo.h"
#include "field/event_gtsnego.h"
#include "field/field_event.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/net_state.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

typedef struct {
    u32 bgm;
    s32 timeout;
    u8 buffer[0x174];
    GameSystem *gsys;
    u32 unk180;
    GtsNegoParam nego;
    PokemonTradeParam trade;
    WifiLoginParam login;
    WifiLogoutParam logout;
    PokeParty *party;
    u32 unk3B0;
} EventGtsNego;

GameEventReturnCode func_ov001_021542c0(GameEvent *event, u32 *state, void *data);
EventGtsNego *func_ov001_02154560(GameEvent *event, GameSystem *gsys, Field *field, u32 unk3B0);

// pokebw2's EventGtsNego_Callback
GameEventReturnCode func_ov001_021542c0(GameEvent *event, u32 *state, void *data) {
    EventGtsNego *wk = data;
    GameSystem *gsys = wk->gsys;
    GameData *gameData = GSYS_GetGameData(gsys);
    Field *field = GSYS_GetField(gsys);

    switch (*state) {
    case 0:
        wk->bgm = GFL_SndBGMGetID();
        GFL_SndBGMFadeOut(6);
        (*state)++;
        break;
    case 1:
        // Wait for the comm system to shut down
        if (!GameCommSys_BootCheck(GSYS_GetGameCommSystem(gsys))) {
            GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, field));
            (*state)++;
        }
        break;
    case 2:
        wk->login.buffer = wk->buffer;
        wk->login.unkC = 0x33;
        GSYS_QueueProc(gsys, OVERLAY_WIFILOGIN, &data_ov115_021d6338, &wk->login);
        (*state)++;
        break;
    case 3:
        if (!GSYS_GetProcMgrState(gsys)) {
            if (wk->login.result == 0) {
                (*state)++;
            } else {
                *state = 14;
            }
        }
        break;
    case 4:
        wk->nego.result = 0;
        GSYS_QueueProc(gsys, OVERLAY_GTSNEGO, &data_ov120_021d890c, &wk->nego);
        (*state)++;
        break;
    case 5:
        if (!GSYS_GetProcMgrState(gsys)) {
            if (wk->nego.result == GTSNEGO_RESULT_EXIT) {
                *state = 12;
            } else if (wk->nego.result == GTSNEGO_RESULT_RETRY_LOGIN) {
                wk->login.unk14 = 1;
                *state = 2;
            } else {
                wk->trade.next = 0;
                (*state)++;
            }
        }
        break;
    case 6:
        wk->trade.gameData = gameData;
        wk->trade.nego = &wk->nego;
        wk->trade.buffer = wk->buffer;
        GSYS_QueueProc(gsys, OVERLAY_POKEMONTRADE, &data_ov119_021e759c, &wk->trade);
        (*state)++;
        break;
    case 7:
        if (!GSYS_GetProcMgrState(gsys)) {
            if (!func_0203cd98()) {
                wk->login.unk14 = 1;
                *state = 2;
            } else if (wk->trade.next == TRADE_NEXT_NEGOTIATE) {
                *state = 4;
            } else if (wk->trade.next == TRADE_NEXT_EVOLVE) {
                *state = 10;
            } else {
                func_02039c68(func_020399a8(), 1, 12);
                func_0203d7b0(FALSE);
                func_0203d7bc(FALSE);
                wk->timeout = 100;
                *state = 8;
            }
        }
        break;
    case 8:
        wk->timeout--;
        if (func_02039cb4(func_020399a8(), 1, 12)) {
            *state = 9;
        }
        if (wk->timeout < 0) {
            *state = 9;
        }
        break;
    case 9:
        func_0203c554(1);
        *state = 4;
        break;
    case 10: {
        ShinkaDemoParam *evolution =
            GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(ShinkaDemoParam), FALSE, "event_gtsnego.c", 204);

        evolution->gameData = wk->trade.gameData;
        evolution->party = wk->party;
        evolution->species = wk->trade.evolveSpecies;
        evolution->partyIndex = 0;
        evolution->method = wk->trade.evolveMethod;
        evolution->unkC = 1;
        evolution->canCancel = FALSE;
        wk->trade.evolution = evolution;
        GSYS_QueueProc(gsys, OVERLAY_SHINKA_DEMO, &data_ov195_02208c40, evolution);
        (*state)++;
        break;
    }
    case 11:
        if (!GSYS_GetProcMgrState(gsys)) {
            GFL_HeapFree(wk->trade.evolution);
            if (!func_0203cd98()) {
                wk->login.unk14 = 1;
                *state = 2;
            } else {
                wk->trade.next = TRADE_NEXT_EVOLVE;
                *state = 6;
            }
        }
        break;
    case 12:
        GSYS_QueueProc(gsys, OVERLAY_WIFILOGIN, &data_ov115_021d654c, &wk->logout);
        (*state)++;
        break;
    case 13:
        if (!GSYS_GetProcMgrState(gsys)) {
            (*state)++;
        }
        break;
    case 14:
        GFL_HeapFree(wk->nego.playerInfo);
        GFL_HeapFree(wk->nego.playerInfo2);
        GFL_HeapFree(wk->party);
        GFL_SndBGMPlay(wk->bgm, SND_CHANNEL_MASK_ALL);
        GFL_SndBGMFadeIn(60);
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        (*state)++;
        break;
    case 15:
        (*state)++;
        break;
    case 16:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

// pokebw2's EventGtsNego_Init
EventGtsNego *func_ov001_02154560(GameEvent *event, GameSystem *gsys, Field *field, u32 unk3B0) {
    EventGtsNego *wk;
    PlayerInfo *playerInfo;

    if (GameCommSys_BootCheck(GSYS_GetGameCommSystem(gsys))) {
        func_02026400(GSYS_GetGameCommSystem(gsys));
    }

    wk = GameEvent_GetData(event);
    wk->gsys = gsys;
    wk->unk3B0 = unk3B0;
    wk->party = PokeParty_Create(HEAPID_GAMEEVENT);
    wk->trade.party = wk->party;
    MI_CpuFill8(&wk->nego, 0, sizeof(GtsNegoParam));
    wk->nego.gameData = GSYS_GetGameData(gsys);
    wk->nego.playerInfo = GFL_HeapAllocate(HEAPID_GAMEEVENT, func_02008468(), TRUE, "event_gtsnego.c", 291);
    wk->nego.playerInfo2 = GFL_HeapAllocate(HEAPID_GAMEEVENT, func_02008468(), TRUE, "event_gtsnego.c", 292);
    playerInfo = GetGameDataPlayerInfo(GSYS_GetGameData(gsys));
    MI_CpuCopy8(playerInfo, wk->nego.playerInfo, func_02008468());

    MI_CpuFill8(&wk->login, 0, sizeof(WifiLoginParam));
    wk->login.gameData = GSYS_GetGameData(gsys);
    wk->login.unk4 = 0;
    wk->login.unk8 = 1;
    wk->logout.gameData = GSYS_GetGameData(gsys);
    wk->logout.unk4 = 0;
    wk->logout.unk8 = 1;
    return wk;
}

// pokebw2's EventGtsNego_Create
GameEvent *func_ov001_0215466c(GameSystem *gsys, Field *field) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov001_021542c0, sizeof(EventGtsNego));

    func_ov001_02154560(event, gsys, field, 0);
    return event;
}

// pokebw2's EventGtsNego_CreateFromArgs. Called through GameEvent_CreateOverlayDelegate, by the
// NetConnectGTSNegotiation script command, which passes the field instead of a pointer to arguments
GameEvent *func_ov001_02154694(GameSystem *gsys, void *field) {
    return func_ov001_0215466c(gsys, field);
}
