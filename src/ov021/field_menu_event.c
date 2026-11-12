// Black's field menu event (pokebw2's field_menu_event.c): ov021 0x021945a4-0x02194670.

#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/subscreen.h"
#include "system/game_system.h"
#include "system/game_data.h"

// The work of the return to the field menu (pokebw2's EventFieldMenuReturn)
typedef struct EventFieldMenuReturn {
    GameEvent *event;
    GameSystem *gsys;
    Field *field;
    u32 screenId;
    u8 seq;
} EventFieldMenuReturn;

GameEventReturnCode EventFieldMenuReturn_Callback(GameEvent *event, u32 *state, void *data);

GameEvent *EventFieldMenuReturn_Create(GameSystem *gsys, Field *field, u32 screenId) {
    GameEvent *event;
    EventFieldMenuReturn *work;

    Field_GetSubscreen(field);
    event = GameEvent_Create(gsys, NULL, EventFieldMenuReturn_Callback, sizeof(EventFieldMenuReturn));
    work = GameEvent_GetData(event);
    MI_CpuFill8(work, 0, sizeof(EventFieldMenuReturn));
    work->gsys = gsys;
    work->event = event;
    work->field = field;
    work->screenId = screenId;
    return event;
}

GameEventReturnCode EventFieldMenuReturn_Callback(GameEvent *event, u32 *state, void *data) {
    EventFieldMenuReturn *work = data;
    GameData *gameData;
    MMSys *mmSys;

    switch (work->seq) {
    case 0:
        if (FieldSubscreen_IsReady(Field_GetSubscreen(work->field)) == TRUE) {
            mmSys = Field_GetActorSystem(work->field);
            gameData = GSYS_GetGameData(GameEvent_GetGameSystem(event));
            DisableAllActorsMovement(mmSys);
            func_020129d0(gameData, 1);
            FieldSubscreen_ReqChange(Field_GetSubscreen(work->field), work->screenId);
            work->seq++;
        }
        break;
    case 1:
        if (FieldSubscreen_IsReady(Field_GetSubscreen(work->field)) == TRUE) {
            func_020129b8(GSYS_GetGameData(GameEvent_GetGameSystem(event)), work->screenId);
            work->seq++;
        }
        break;
    case 2:
        EnableAllActorsMovement(Field_GetActorSystem(work->field));
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}
