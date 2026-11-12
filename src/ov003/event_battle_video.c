// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov003/event_battle_video.c, which is the same
// code in Black 2/White 2's overlay 3. The names are Black's dsd names and swan's; pokebw2's guesses are in the
// comments.
#include "types.h"
#include "app/battle_video.h"
#include "field/event_battle_video.h"
#include "field/field_event.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "system/game_comm.h"
#include "system/game_event.h"
#include "system/game_system.h"

typedef struct {
    u32 bgm;
    GameSystem *gsys;
    Field *field;
    BattleVideoParam param;
    u32 mode;
    u32 unk18;
} EventBattleVideo;

GameEventReturnCode func_ov003_021542c0(GameEvent *event, u32 *state, void *data);

// pokebw2's EventBattleVideo_Callback
GameEventReturnCode func_ov003_021542c0(GameEvent *event, u32 *state, void *data) {
    EventBattleVideo *wk = data;
    GameSystem *gsys = wk->gsys;

    switch (*state) {
    case 0:
        // Wait for the comm system to shut down
        if (!GameCommSys_BootCheck(GSYS_GetGameCommSystem(gsys))) {
            wk->bgm = GFL_SndBGMGetID();
            GFL_SndBGMFadeOut(6);
            (*state)++;
        }
        break;
    case 1:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, wk->field));
        *state = 2;
        break;
    case 2:
        wk->param.gameData = GSYS_GetGameData(gsys);
        wk->param.mode = wk->mode;
        GSYS_QueueProc(gsys, OVERLAY_NONE, &data_0209defc, &wk->param);
        (*state)++;
        break;
    case 3:
        if (!GSYS_GetProcMgrState(gsys)) {
            (*state)++;
        }
        break;
    case 4:
        (*state)++;
        break;
    case 5:
        GFL_SndBGMPlay(wk->bgm, SND_CHANNEL_MASK_ALL);
        GFL_SndBGMFadeIn(60);
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        (*state)++;
        break;
    case 6:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

// pokebw2's EventBattleVideo_Create
GameEvent *func_ov003_0215437c(GameSystem *gsys, Field *field, u32 mode) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov003_021542c0, sizeof(EventBattleVideo));
    EventBattleVideo *wk;

    if (GameCommSys_BootCheck(GSYS_GetGameCommSystem(gsys))) {
        func_02026400(GSYS_GetGameCommSystem(gsys));
    }

    wk = GameEvent_GetData(event);
    wk->gsys = gsys;
    wk->field = field;
    wk->mode = mode;
    return event;
}

// pokebw2's EventBattleVideo_CreateFromArgs. Called through GameEvent_CreateOverlayDelegate, by the
// NetConnectBattleVideo script command
GameEvent *func_ov003_021543bc(GameSystem *gsys, void *data) {
    EventBattleVideoArgs *args = data;

    return func_ov003_0215437c(gsys, args->field, args->mode);
}
