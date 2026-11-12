// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/event_battle_lose.c, which is the same
// code in Black 2/White 2's overlay 12, except that Black's callback doesn't call func_ov012_02160618 and runs script 3
// where Black 2 runs script 2. Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0).
#include "types.h"
#include "field/event_battle_lose.h"
#include "field/event_game_clear.h"
#include "field/field_script.h"
#include "field/pleasure_boat.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "system/brightness.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

// The event of a lost battle: the screen that says so, the blackout and the script that follows

GameEventReturnCode EventBattleLose_Callback(GameEvent *event, u32 *state, void *data) {
    BattleLoseData *work = data;

    switch (*state) {
    case 0:
        func_02023f60(-16, 0x37, BRIGHTNESS_MAIN_SCREEN);
        func_02023f60(-16, 0x3f, BRIGHTNESS_SUB_SCREEN);
        GSYS_QueueProcAsEvent(event, OVERLAY_ID(205), &data_ov205_021b9ec0, work);
        (*state)++;
        break;
    case 1:
        func_02023f60(0, 0x3f, BRIGHTNESS_BOTH_SCREENS);
        GameEvent_ChainNext(event, EventMapChangeBlackout_CreateExternal(work->gsys));
        (*state)++;
        break;
    case 2:
        if (!work->returnNonLeague) {
            EventScriptCall_Start(event, 3, NULL, 0, HEAPID_FIELDMAP);
        } else {
            EventScriptCall_Start(event, 0x7d1, NULL, 0, HEAPID_FIELDMAP);
        }
        (*state)++;
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *EventBattleLose_Create(GameSystem *gsys) {
    GameData *gameData = GSYS_GetGameData(gsys);
    GameEvent *event = GameEvent_Create(gsys, NULL, EventBattleLose_Callback, sizeof(BattleLoseData));
    BattleLoseData *data = GameEvent_GetData(event);
    data->gsys = gsys;
    data->returnNonLeague = IsReturnLocationNonLeaguePokeCen(gameData);
    data->playerInfo = GetGameDataPlayerInfo(gameData);
    func_ov010_021666a4(func_02012ed0(gameData));
    func_0201213c(gsys, 0);
    return event;
}
