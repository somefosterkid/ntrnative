// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov033/event_cgear_shutdown.c, which is the same
// code in Black 2/White 2's overlay 33. Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0).
#include "types.h"
#include "field/event_cgear_shutdown.h"
#include "field/field.h"
#include "field/subscreen.h"
#include "struct_decls.h"
#include "system/game_event.h"
#include "system/game_system.h"

static GameEventReturnCode EventCGearShutdown_Callback(GameEvent *event, u32 *state, void *eventData);

GameEvent *EventCGearShutdown_Create(GameSystem *gsys) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventCGearShutdown_Callback, sizeof(struct CGearShutdownData));
    struct CGearShutdownData *data = GameEvent_GetData(event);
    Field *field = GSYS_GetField(gsys);

    data->field = field;
    data->subscreen = Field_GetSubscreen(field);
    return event;
}

static GameEventReturnCode EventCGearShutdown_Callback(GameEvent *event, u32 *state, void *eventData) {
    struct CGearShutdownData *data = eventData;

    switch (*state) {
    case 0:
        FieldSubscreen_ReqChange(data->subscreen, 9);
        (*state)++;
        break;
    case 1:
        if (FieldSubscreen_GetReturnSubscreen(data->subscreen) != 0xe) {
            break;
        }
        func_ov021_021a0fa8(data->subscreen);
        (*state)++;
        break;
    case 2:
        FieldSubscreen_ReqChange(data->subscreen, 0);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}
