// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/event_field_menu.c, Black 2/White 2's
// overlay 12. Black's menu has no screenId in the app call, keeps subscreens 3 and 4 where Black 2 keeps 4 and 10, and
// has no state 9 (Black 2's script 0x2a01). Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0).
#include "types.h"
#include "field/app_call.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_menu.h"
#include "field/hidden_event.h"
#include "field/itemuse_event.h"
#include "field/player_action.h"
#include "field/subscreen.h"
#include "field/zone.h"
#include "gfl/std.h"
#include "gfl/ui.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

// The event of the field menu on the touch screen, which opens the apps the player chooses

GameEvent *EventFieldMenu_Create(GameSystem *gsys, Field *field, u16 param) {
    FieldSubscreen *subscreen = Field_GetSubscreen(field);
    GameEvent *event = GameEvent_Create(gsys, NULL, EventFieldMenu_Callback, sizeof(FieldMenuWork));
    FieldMenuWork *work = GameEvent_GetData(event);
    u32 screenId;

    MI_CpuFill8(work, 0, sizeof(FieldMenuWork));
    work->gameSystem = gsys;
    work->event = event;
    work->field = field;
    work->code = param;
    work->state = 0;
    work->appCall.gameSystem = gsys;
    work->appCall.field = field;
    work->appCall.parent = event;
    work->appCall.canRetry = func_ov010_0215fed4;
    work->appCall.callback1 = func_ov010_0215fef0;
    work->appCall.callback2 = func_ov010_0215fef4;
    work->appCall.arg = work;
    work->appCall.appParam = -1;
    screenId = FieldSubscreen_GetScreenID(subscreen);
    switch (screenId) {
    case 3:
        work->screenId = 3;
        break;
    case 4:
        work->screenId = 4;
        break;
    default:
        work->screenId = FieldSubscreen_GetIDForChange(Field_GetSubscreen(work->field), 0);
        break;
    }
    func_020355d8(TRUE);
    return event;
}

GameEvent *EventFieldMenu_CreateUnionRoom(GameSystem *gsys, Field *field, u16 param) {
    GameEvent *event = EventFieldMenu_Create(gsys, field, param);
    FieldMenuWork *work = GameEvent_GetData(event);
    u16 zoneId = Field_GetPlayerStateZoneID(field);

    if (IsZone150Or151(zoneId) == 1) {
        work->screenId = 5;
    } else {
        work->screenId = 2;
    }
    return event;
}

GameEventReturnCode EventFieldMenu_Callback(GameEvent *event, u32 *state, void *data) {
    FieldMenuWork *work = data;
    MMSys *actorSystem;
    GameData *gameData;
    u32 selection;
    u32 returnSubscreen;
    PlayerActionPossibilities action;
    HiddenEventArgs args;
    GameEvent *next;

    switch (work->state) {
    case 0:
        if (FieldSubscreen_IsReady(Field_GetSubscreen(work->field)) == TRUE) {
            actorSystem = Field_GetActorSystem(work->field);
            gameData = GSYS_GetGameData(GameEvent_GetGameSystem(event));
            DisableAllActorsMovement(actorSystem);
            func_020129d0(gameData, 1);
            FieldSubscreen_ReqChange(Field_GetSubscreen(work->field), 1);
            work->state = 1;
        }
        break;
    case 1:
        returnSubscreen = FieldSubscreen_GetReturnSubscreen(Field_GetSubscreen(work->field));
        if (returnSubscreen == 3) {
            work->state = 2;
            func_ov021_021a0fa8(Field_GetSubscreen(work->field));
        } else if (returnSubscreen == 4) {
            work->state = 3;
            func_ov021_021a0fa8(Field_GetSubscreen(work->field));
        }
        break;
    case 2:
        gameData = GSYS_GetGameData(GameEvent_GetGameSystem(event));
        selection = func_ov021_021a12a4(Field_GetSubscreen(work->field));
        func_020129d0(gameData, selection);
        work->unk10 = func_ov021_021a12a4(Field_GetSubscreen(work->field));
        work->appCall.appId = func_ov010_0215fec8(selection);
        work->state = 5;
        break;
    case 3:
        EnableAllActorsMovement(Field_GetActorSystem(work->field));
        if (work->appCall.eventType != 1 && work->appCall.eventType != 4) {
            GameEvent_Replace(event,
                              EventFieldMenuReturn_Create(GameEvent_GetGameSystem(event), work->field, work->screenId));
            return GAMEEVENT_CONTINUE;
        }
        return GAMEEVENT_DONE;
    case 4:
        EnableAllActorsMovement(Field_GetActorSystem(work->field));
        CalcPlayerActionPossibilities(GSYS_GetField(work->gameSystem), &action);
        func_ov010_0215ea30(&args, work->appCall.partySlot, work->appCall.eventId, work->appCall.eventValue);
        next = CreateHidenEvent(work->appCall.eventId, &args, &action);
        if (next == NULL) {
            return GAMEEVENT_DONE;
        }
        GameEvent_Replace(work->event, next);
        break;
    case 5:
        GameEvent_ChainNext(event, EventFieldAppCall_Create(&work->appCall, work->code));
        work->state = 6;
        break;
    case 6:
        if (work->appCall.eventType == 0) {
            work->state = 1;
        } else if (work->appCall.eventType == 1) {
            work->state = 3;
        } else if (work->appCall.eventType == 3) {
            work->state = 7;
        } else if (work->appCall.eventType == 2) {
            work->state = 4;
        } else if (work->appCall.eventType == 4) {
            work->state = 3;
        }
        break;
    case 7:
        EnableAllActorsMovement(Field_GetActorSystem(work->field));
        GameEvent_ChainNext(event, CallFieldCommonEventFunc(work->appCall.eventId, work->gameSystem, work->field));
        work->state = 8;
        break;
    case 8:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}
