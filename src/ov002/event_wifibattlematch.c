// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov002/event_wifibattlematch.c, the same code
// as Black 2/White 2's overlay 2 but for Black's overlay 199 and a step that does nothing where Black 2 clears the
// record of a match in progress. The names are Black's dsd names and swan's; pokebw2's guesses are in the comments.
#include "types.h"
#include "app/wifibattlematch.h"
#include "field/event_wifibattlematch.h"
#include "field/field_event.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "system/game_comm.h"
#include "system/game_event.h"
#include "system/game_system.h"

typedef struct {
    u32 bgm;
    GameSystem *gsys;
    Field *field;
    WifiBattleMatchParam *param;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    // Never set, so the field transitions are skipped
    BOOL useTransitions;
} EventWifiBattleMatch;

GameEventReturnCode func_ov002_021542c0(GameEvent *event, u32 *state, void *data);

// pokebw2's EventWifiBattleMatch_Callback
GameEventReturnCode func_ov002_021542c0(GameEvent *event, u32 *state, void *data) {
    EventWifiBattleMatch *wk = data;
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
        if (wk->useTransitions) {
            GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, wk->field, 0, 0));
        }
        (*state)++;
        break;
    case 2:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, wk->field));
        (*state)++;
        break;
    case 3: {
        WifiBattleMatchParam *param =
            GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(WifiBattleMatchParam), TRUE, "event_wifibattlematch.c", 134);

        wk->param = param;
        param->unk8 = wk->unk10;
        param->gameData = GSYS_GetGameData(gsys);
        param->unk4 = wk->unk18;
        param->unkC = wk->unk14;
        GSYS_QueueProc(gsys, OVERLAY_WIFIBATTLEMATCH, &data_ov199_02213c4c, wk->param);
        (*state)++;
        break;
    }
    case 4:
        if (!GSYS_GetProcMgrState(gsys)) {
            GFL_HeapFree(wk->param);
            (*state)++;
        }
        break;
    case 5:
        (*state)++;
        break;
    case 6:
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        GFL_SndBGMPlay(wk->bgm, SND_CHANNEL_MASK_ALL);
        GFL_SndBGMFadeIn(60);
        (*state)++;
        break;
    case 7:
        if (wk->useTransitions) {
            GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, wk->field, 0, 0, 1, 0, 0));
        }
        (*state)++;
        break;
    case 8:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

// pokebw2's EventWifiBattleMatch_Create
GameEvent *func_ov002_021543e8(GameSystem *gsys, Field *field, u32 unk10, u32 unk14, u32 unk18) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov002_021542c0, sizeof(EventWifiBattleMatch));
    EventWifiBattleMatch *wk;

    if (GameCommSys_BootCheck(GSYS_GetGameCommSystem(gsys))) {
        func_02026400(GSYS_GetGameCommSystem(gsys));
    }

    wk = GameEvent_GetData(event);
    wk->gsys = gsys;
    wk->field = field;
    wk->unk10 = (u16)unk10;
    wk->unk14 = unk14;
    wk->unk18 = unk18;
    wk->useTransitions = FALSE;
    return event;
}

// pokebw2's EventWifiBattleMatch_CreateFromArgs. Called through GameEvent_CreateOverlayDelegate, by the
// NetConnectWiFiBattle script command
GameEvent *func_ov002_02154438(GameSystem *gsys, void *data) {
    EventWifiBattleMatchArgs *args = data;

    return func_ov002_021543e8(gsys, args->field, args->unk4, args->unk8, args->unkC);
}
