// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/event_cgear_poweron.c, ported by tools/port_file.py.
// The event that turns the C-Gear on: it stops the game's communication, starts it again when asked to, and changes the
// subscreen to the C-Gear. Function names from swan; the file's name is descriptive, after event_cgear_shutdown.c
#include "types.h"
#include "field/event_cgear_poweron.h"
#include "field/field.h"
#include "field/subscreen.h"
#include "gfl/std.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

static void EventCGearPowerOn_SubscreenCallback(void *work) {
    CGearPowerOnData *data = work;

    data->subscreenChanged = TRUE;
}

void func_02026400(GameCommSys *comm);
void func_02012158(GameSystem *gsys, u32 param);
GameEventReturnCode EventCGearPowerOn_Callback(GameEvent *event, u32 *state, void *work) {
    CGearPowerOnData *data = work;
    GameSystem *gsys = data->gsys;
    FieldSubscreen *subscreen = Field_GetSubscreen(GSYS_GetField(gsys));
    GameCommSys *comm = GSYS_GetGameCommSystem(gsys);

    switch (*state) {
    case 0:
        if (GameCommSys_BootCheck(comm)) {
            func_02026400(comm);
        }
        (*state)++;
        break;
    case 1:
        if (!GameCommSys_BootCheck(comm)) {
            func_02012158(gsys, 0);
            (*state)++;
        }
        break;
    case 2:
        if (data->bootComm) {
            func_02012158(gsys, 1);
            func_020121cc(gsys);
        }
        (*state)++;
        break;
    case 3:
        func_ov021_021a0f7c(subscreen, 8, EventCGearPowerOn_SubscreenCallback, data);
        (*state)++;
        break;
    case 4:
        if (data->subscreenChanged) {
            (*state)++;
        }
        break;
    case 5:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *EventCGearPowerOn_Create(GameSystem *gsys, BOOL bootComm) {
    GSYS_GetGameData(gsys);
    GameEvent *event = GameEvent_Create(gsys, NULL, EventCGearPowerOn_Callback, sizeof(CGearPowerOnData));
    CGearPowerOnData *data = GameEvent_GetData(event);

    MI_CpuFill8(data, 0, sizeof(CGearPowerOnData));
    data->gsys = gsys;
    data->bootComm = bootComm;
    return event;
}



