// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov004/event_worldtrade.c, which is the same
// code in Black 2/White 2's overlay 4, with Black's overlay 135. The names are Black's dsd names and swan's; pokebw2's
// guesses are in the comments.
#include "types.h"
#include "app/worldtrade.h"
#include "field/event_worldtrade.h"
#include "field/field_event.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "save/pokedex.h"
#include "save/save_control.h"
#include "save/trainer_data_savedata.h"
#include "save/wifi_list.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

typedef struct {
    u32 bgm;
    GameSystem *gsys;
    Field *field;
    WorldTradeParam param;
    // Never set here, so the field transitions are skipped
    BOOL useTransitions;
} EventWorldTrade;

GameEventReturnCode func_ov004_02154390(GameEvent *event, u32 *state, void *data);

// pokebw2's EventWorldTrade_CreateFromArgs. Called through GameEvent_CreateOverlayDelegate, by the NetConnectGTS
// script command
GameEvent *func_ov004_021542c0(GameSystem *gsys, void *data) {
    EventWorldTradeArgs *args = data;

    return func_ov004_021542cc(gsys, args->field, args->unused);
}

// pokebw2's EventWorldTrade_Create
GameEvent *func_ov004_021542cc(GameSystem *gsys, Field *field, u32 unused) {
    GameData *gameData = GSYS_GetGameData(gsys);
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov004_02154390, sizeof(EventWorldTrade));
    EventWorldTrade *wk;

    if (GameCommSys_BootCheck(GSYS_GetGameCommSystem(gsys))) {
        func_02026400(GSYS_GetGameCommSystem(gsys));
    }

    wk = GameEvent_GetData(event);
    MI_CpuFill8(wk, 0, sizeof(EventWorldTrade));
    wk->gsys = gsys;
    wk->field = field;
    wk->param.savedata = GameData_GetSaveControl(gameData);
    wk->param.worldtrade_data = func_0200ad6c(wk->param.savedata);
    wk->param.systemdata = getSaveAdventureDataBlk(wk->param.savedata);
    wk->param.myparty = SaveControl_GetPokePartySave(wk->param.savedata);
    wk->param.mybox = GameData_GetBoxSaveAccessor(gameData);
    wk->param.pokedex = GameData_GetPokedex(gameData);
    wk->param.wifilist = func_0201283c(gameData);
    wk->param.wifihistory = getUnityTower_SurveySaveBlkAddrress(wk->param.savedata);
    wk->param.mystatus = func_02008730(wk->param.savedata);
    wk->param.config = getTrainerDataBlkAddress(wk->param.savedata);
    wk->param.record = getTrainerCardInfoBlkAddress(wk->param.savedata);
    wk->param.bag = GameData_GetBag(gameData);
    wk->param.gsys = gsys;
    wk->param.isNationalDex = PokeDex_IsNationalObtained(wk->param.pokedex);
    wk->param.profileId = func_02009cc0(wk->param.wifilist);
    wk->param.unk34 = 0;
    wk->param.unk38 = 0;
    return event;
}

// pokebw2's EventWorldTrade_Callback
GameEventReturnCode func_ov004_02154390(GameEvent *event, u32 *state, void *data) {
    EventWorldTrade *wk = data;

    switch (*state) {
    case 0:
        // Wait for the comm system to shut down
        if (!GameCommSys_BootCheck(GSYS_GetGameCommSystem(wk->gsys))) {
            *state = 1;
        }
        break;
    case 1:
        if (wk->useTransitions) {
            GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(wk->gsys, wk->field, 0, 0));
        }
        wk->bgm = GFL_SndBGMGetID();
        GFL_SndBGMFadeOut(6);
        *state = 2;
        break;
    case 2:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(wk->gsys, wk->field));
        *state = 3;
        break;
    case 3:
        GSYS_QueueProc(wk->gsys, OVERLAY_WORLDTRADE, &data_ov135_02202bac, &wk->param);
        *state = 4;
        break;
    case 4:
        if (!GSYS_GetProcMgrState(wk->gsys)) {
            GFL_SndBGMPlay(wk->bgm, SND_CHANNEL_MASK_ALL);
            GFL_SndBGMFadeIn(60);
            GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(wk->gsys));
            *state = 5;
        }
        break;
    case 5:
        if (wk->useTransitions) {
            GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(wk->gsys, wk->field, 0, 0, 1, 0, 0));
        }
        *state = 6;
        break;
    case 6:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}
