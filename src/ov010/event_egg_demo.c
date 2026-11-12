// Black's code at ov010 0x02166b58-0x02166c7c: pokebw2's ov016 event_egg_demo.c (EventEggDemo_Callback and
// EventEggDemo_Create, the egg hatching demo event, which queues overlay 215's egg_demo proc here).
#include "types.h"
#include "constants/pokemon.h"
#include "demo/egg_demo.h"
#include "field/event_egg_demo.h"
#include "field/field_event.h"
#include "field/game_beacon_set.h"
#include "pml/poke_party.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "gfl/overlay.h"

extern u32 OVERLAY_215_ID[];
extern const GameProcFunctions data_ov215_021f7118;

typedef struct EventEggDemo {
    GameSystem *gsys;
    GameData *gameData;
    Field *field;
    PartyPkm *pkm;
    EggDemoParam demo;
} EventEggDemo;

GameEventReturnCode func_ov010_02166b58(GameEvent *event, u32 *state, void *data);

GameEventReturnCode func_ov010_02166b58(GameEvent *event, u32 *state, void *data) {
    EventEggDemo *work = data;
    GameSystem *gsys = work->gsys;
    GameData *gameData = work->gameData;
    Field *field = work->field;

    switch (*state) {
    case 0:
        if (!PokeParty_GetParam(work->pkm, PKM_PARAM_IS_EGG, NULL) || PokeParty_GetParam(work->pkm, 3, NULL) == 1) {
            *state = 7;
        } else {
            *state = 1;
        }
        break;
    case 1:
        work->demo.gameData = gameData;
        work->demo.pkm = work->pkm;
        *state = 2;
        break;
    case 2:
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, 0, 0));
        *state = 3;
        break;
    case 3:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, field));
        *state = 4;
        break;
    case 4:
        GSYS_QueueProc(gsys, OVERLAY_ID(215), &data_ov215_021f7118, &work->demo);
        *state = 5;
        break;
    case 5:
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        *state = 6;
        break;
    case 6:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, 0, 0, 1, 0, 0));
        func_ov010_0216559c((u16)PokeParty_GetParam(work->pkm, PKM_PARAM_SPECIES, NULL));
        *state = 7;
        break;
    case 7:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *func_ov010_02166c4c(GameSystem *gsys, Field *field, PartyPkm *pkm) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov010_02166b58, sizeof(EventEggDemo));
    EventEggDemo *work = GameEvent_GetData(event);

    work->gsys = gsys;
    work->field = field;
    work->gameData = GSYS_GetGameData(gsys);
    work->pkm = pkm;
    return event;
}
