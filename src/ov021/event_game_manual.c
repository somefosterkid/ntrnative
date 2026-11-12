// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov033/event_game_manual.c, ported by tools/port_file.py.
#include "types.h"
#include "app/manual.h"
#include "field/event_game_manual.h"
#include "field/field_event.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "struct_decls.h"
#include "system/game_event.h"
#include "system/game_system.h"

GameEvent *func_ov021_021d30b8(GameSystem *gsys, u16 *result) {
    GameData *gameData;
    Field *field;
    GameEvent *event;
    GameManualEventWork *work;
    GameManualSubwork *subwork;

    gameData = GSYS_GetGameData(gsys);
    field = GSYS_GetField(gsys);
    event = GameEvent_Create(gsys, NULL, func_ov021_021d310c, sizeof(GameManualEventWork));
    work = GameEvent_GetData(event);
    work->gsys = gsys;
    work->field = field;
    work->result = result;
    subwork = GFL_HeapAllocate(4, sizeof(GameManualSubwork), FALSE, "event_game_manual.c", 0x4a);
    work->subwork = subwork;
    subwork->gameData = gameData;
    return event;
}

GameEvent *func_020146f8(GameSystem *gsys, Field *field, u16 id, void *data, GameManualSubwork *subwork);
extern u32 OVERLAY_225_ID[];
extern u32 OVERLAY_225_ID[];
GameEventReturnCode func_ov021_021d310c(GameEvent *event, u32 *state, void *data) {
    GameManualEventWork *work;
    GameEvent *next;

    work = data;
    switch (*state) {
    case 0:
        next = EventFieldSubprocessTransition_Create(work->gsys, work->field, OVERLAY_ID(225), &data_ov225_021bbde4, work->subwork);
        GameEvent_ChainNext(event, next);
        (*state)++;
        break;
    case 1:
        func_ov021_021d3164(work);
        GFL_HeapFree(work->subwork);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


void func_ov021_021d3164(GameManualEventWork *work) {
    if (work->subwork->result == 0) {
        *work->result = 0;
    } else {
        *work->result = 1;
    }
}
