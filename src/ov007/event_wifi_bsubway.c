// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov009/event_wifi_bsubway.c, which is the same
// code in Black 2/White 2's overlay 9, with Black's overlay 116. The names are Black's dsd names and swan's; pokebw2's
// guesses are in the comments.
#include "types.h"
#include "app/wifi_bsubway.h"
#include "field/event_wifi_bsubway.h"
#include "field/field_event.h"
#include "system/game_comm.h"
#include "system/game_event.h"
#include "system/game_system.h"

// Also the parameter of the Wi-Fi Battle Subway proc, which sets procResult
typedef struct {
    u32 mode;
    GameSystem *gsys;
    u32 procResult;
    u16 *result;
} EventWifiBSubway;

GameEventReturnCode func_ov007_02154310(GameEvent *event, u32 *state, void *data);

// pokebw2's EventWifiBSubway_CreateFromArgs. Called through GameEvent_CreateOverlayDelegate, by a script command
GameEvent *func_ov007_021542c0(GameSystem *gsys, void *data) {
    EventWifiBSubwayArgs *args = data;

    return func_ov007_021542cc(gsys, args->mode, args->result);
}

// pokebw2's EventWifiBSubway_Create
GameEvent *func_ov007_021542cc(GameSystem *gsys, u32 mode, u16 *result) {
    GSYS_GetGameData(gsys);
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov007_02154310, sizeof(EventWifiBSubway));
    EventWifiBSubway *wk = GameEvent_GetData(event);

    wk->mode = mode;
    wk->gsys = gsys;
    wk->result = result;
    if (GameCommSys_BootCheck(GSYS_GetGameCommSystem(gsys))) {
        func_02026400(GSYS_GetGameCommSystem(gsys));
    }
    return event;
}

// pokebw2's EventWifiBSubway_Callback
GameEventReturnCode func_ov007_02154310(GameEvent *event, u32 *state, void *data) {
    EventWifiBSubway *wk = data;
    GameSystem *gsys = wk->gsys;
    Field *field = GSYS_GetField(gsys);

    switch (*state) {
    case 0:
        // Wait for the comm system to shut down
        if (!GameCommSys_BootCheck(GSYS_GetGameCommSystem(gsys))) {
            (*state)++;
        }
        break;
    case 1:
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, 0, 0));
        (*state)++;
        break;
    case 2:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, field));
        (*state)++;
        break;
    case 3:
        GSYS_QueueProc(gsys, OVERLAY_WIFI_BSUBWAY, &data_ov116_021d83a4, wk);
        (*state)++;
        break;
    case 4:
        if (!GSYS_GetProcMgrState(gsys)) {
            if (wk->procResult == 0) {
                *wk->result = TRUE;
            } else {
                *wk->result = FALSE;
            }
            (*state)++;
        }
        break;
    case 5:
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        (*state)++;
        break;
    case 6:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, 0, 0, 1, 0, 0));
        (*state)++;
        break;
    case 7:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}
