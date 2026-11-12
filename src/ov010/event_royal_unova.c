// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/event_royal_unova.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/demo3d.h"
#include "field/event_3d_demo.h"
#include "field/field_script.h"
#include "field/pleasure_boat.h"
#include "gfl/heap.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/vm.h"


// The Royal Unova's view of the city at night, a 3D demo (a descriptive name). Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#define PLEASURE_BOAT_CLOCK_PERIOD 1350

typedef struct {
    Demo3DParam param;
    PleasureBoat *boat;
    u16 *result;
} EventRoyalUnovaWork;

u32 func_ov010_02166658(PleasureBoat *boat, u32 room);
static GameEvent *EventRoyalUnova_Create(GameSystem *gsys, PleasureBoat *boat, u16 *result);
static GameEventReturnCode EventRoyalUnova_Callback(GameEvent *event, u32 *state, void *data);
static s16 func_ov010_0216670c(PleasureBoat *boat);
static void func_ov010_02166714(PleasureBoat *boat, u32 OS_GetTick);

u32 func_ov010_02166658(PleasureBoat *boat, u32 room) {
    if (boat == NULL) {
        return 10;
    }
    return boat->rooms[room].objCode;
}

static GameEvent *EventRoyalUnova_Create(GameSystem *gsys, PleasureBoat *boat, u16 *result) {
    GameEvent *event = GameEvent_Create(gsys, NULL, EventRoyalUnova_Callback, sizeof(EventRoyalUnovaWork));
    EventRoyalUnovaWork *work = GameEvent_GetData(event);

    work->boat = boat;
    work->result = result;
    SetupPlaySequenceEventRealTime(&work->param, gsys, 1, 0);
    work->param.unk08 = func_ov010_0216670c(boat);
    return event;
}

// pokebw2's PleasureBoat_Free
void func_ov010_021666a4(PleasureBoat **boat) {
    if (*boat != NULL) {
        GFL_HeapFree(*boat);
        *boat = NULL;
    }
}

static GameEventReturnCode EventRoyalUnova_Callback(GameEvent *event, u32 *state, void *data) {
    EventRoyalUnovaWork *work = GameEvent_GetData(event);

    switch (*state) {
    case 0:
        GSYS_QueueProcAsEvent(event, OVERLAY_DEMO3D, &data_ov203_021bfb3c, work);
        (*state)++;
        break;
    case 1:
        if (work->param.result == 2) {
            *work->result = TRUE;
        } else {
            *work->result = FALSE;
        }
        func_ov010_02166714(work->boat, work->param.unk10);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

static s16 func_ov010_0216670c(PleasureBoat *boat) {
    return boat->OS_GetTick;
}

static void func_ov010_02166714(PleasureBoat *boat, u32 OS_GetTick) {
    s32 period;

    boat->OS_GetTick = OS_GetTick;
    period = boat->OS_GetTick / PLEASURE_BOAT_CLOCK_PERIOD;
    if (boat->period <= period) {
        boat->period = period;
    }
}

BOOL func_ov010_02166734(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    PleasureBoat **boat = func_02012ed0(GSYS_GetGameData(FieldScriptEnv_GetGameSystem(env)));
    u16 room = ScriptReadAny(vm, env);
    u16 kind = VM_Read16(vm);
    u32 index = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);
    switch (kind) {
    case 4:
        *result = func_ov010_02166658(*boat, room);
        break;
    case 5:
        *result = func_ov021_021c87a0(*boat, room);
        break;
    case 6:
        *result = func_ov021_021c87ac(*boat, room);
        break;
    case 7:
        *result = func_ov021_021c87b8(*boat, room, index);
        break;
    case 8:
        if (func_ov021_021c87c8(*boat, room)) {
            *result = TRUE;
        } else {
            *result = FALSE;
        }
        break;
    default:
        *result = 0;
        break;
    }
    return FALSE;
}

BOOL s01A2_CallRoyalUnovaView(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    PleasureBoat **boat = func_02012ed0(GSYS_GetGameData(gsys));
    GameEvent *event = EventRoyalUnova_Create(gsys, *boat, ScriptReadVar(vm, env));

    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}
