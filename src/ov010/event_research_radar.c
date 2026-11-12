// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov018/event_research_radar.c, which is the same
// code in Black 2/White 2's overlay 18, with Black's overlay and names.
#include "types.h"
#include "app/research_radar.h"
#include "field/event_research_radar.h"
#include "field/field_event.h"
#include "gfl/heap.h"
#include "system/game_event.h"
#include "system/game_system.h"

typedef struct {
    GameSystem *gsys;
    Field *field;
    GameSystem **param;
} EventResearchRadar;

static char sFile[] = "event_research_radar.c";

static GameEventReturnCode func_ov010_02166f9c(GameEvent *event, u32 *state, void *data) {
    EventResearchRadar *work = data;
    GameSystem *gsys = work->gsys;
    Field *field = work->field;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, 0, 0));
        *state = 1;
        break;
    case 1:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, field));
        *state = 2;
        break;
    case 2:
        GSYS_QueueProc(gsys, OVERLAY_RESEARCH_RADAR_APP, &data_ov217_021c3fe0, work->param);
        *state = 3;
        break;
    case 3:
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        *state = 4;
        break;
    case 4:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, 0, 0, 1, 0, 0));
        *state = 5;
        break;
    case 5:
        GFL_HeapFree(work->param);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *func_ov010_02167044(GameSystem *gsys, Field *field) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov010_02166f9c, sizeof(EventResearchRadar));
    EventResearchRadar *work = GameEvent_GetData(event);

    work->gsys = gsys;
    work->field = field;
    work->param = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(*work->param), FALSE, sFile, 117);
    *work->param = gsys;
    return event;
}
