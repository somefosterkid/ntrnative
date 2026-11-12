// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/hiden_event.c, ported by tools/port_file.py.
#include "types.h"
#include "field/event_chatot.h"
#include "field/event_fly.h"
#include "field/event_mapchange.h"
#include "field/event_sweet_scent.h"
#include "field/field.h"
#include "field/field_script.h"
#include "field/field_skill_map_eff.h"
#include "field/field_status.h"
#include "field/game_beacon_set.h"
#include "field/hidden_event.h"
#include "field/player_action.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "save/event_work.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "field/field_player.h"
#include "field/field_actor.h"
#include "field/field_map.h"
#include "field/ov021_021c7de0.h"

BOOL func_ov012_02159440(PlayerActionPossibilities *context);

// The places in the Abyssal Ruins where Flash and Strength can be used
static const HiddenArea ABYSSAL_RUINS_FLASH_ROCK_RADIUS = { 14, 26, 1, 2 };
static const HiddenArea ABYSSAL_RUINS_STRENGTH_ROCK_RADIUS = { 40, 26, 1, 2 };

// Four zero bytes that nothing reads, ahead of the Abyssal Ruins areas (pokebw2's, whose ov012 hiden_event.c's .rodata
// starts with them too). Their type and form are a guess
const u32 HIDEN_EVENT_UNUSED = 0;

// The events of each hidden move, by its kind, and the same in the Abyssal Ruins, where Strength and Flash differ
const HiddenEventSpec HIDEN_EVENTS_NORMAL[11] = {
    { EventCutCall_Create, EventCutCall_Check },
    { EventSurfCall_Create, EventSurfCall_Check },
    { EventWaterfallCall_Create, EventWaterfallCall_Check },
    { EventStrengthCall_Create, EventStrengthCall_Check },
    { EventFly_Create, EventFly_Check },
    { EventFlash_Create, EventFlash_Check },
    { EventTeleportCall_Create, EventTeleportCall_Check },
    { EventDigCall_Create, EventDigCall_Check },
    { func_ov010_0215ec40, func_ov010_0215ec2c },
    { func_ov010_0215ef40, func_ov010_0215ef2c },
    { EventDivingCall_Create, EventDivingCall_Check },
};

const HiddenEventSpec HIDEN_EVENTS_RUINS[11] = {
    { EventCutCall_Create, EventCutCall_Check },
    { EventSurfCall_Create, EventSurfCall_Check },
    { EventWaterfallCall_Create, EventWaterfallCall_Check },
    { EventRuinsStrengthCall_Create, EventStrengthCall_Check },
    { EventFly_Create, EventFly_Check },
    { EventRuinsFlash_Create, EventFlash_Check },
    { EventTeleportCall_Create, EventTeleportCall_Check },
    { EventDigCall_Create, EventDigCall_Check },
    { func_ov010_0215ec40, func_ov010_0215ec2c },
    { func_ov010_0215ef40, func_ov010_0215ef2c },
    { EventDivingCall_Create, EventDivingCall_Check },
};


void CalcPlayerActionPossibilities(Field *field, PlayerActionPossibilities *action) {
    VecFx32 position;
    u16 direction;
    MMSys *actorSystem;
    FieldActor *actor;
    u32 tileInFront;
    u32 tileUnder;
    FieldPlayer *player;
    u16 objCode;
    u16 diveZone;
    s16 x;
    s16 y;
    s16 z;

    MI_CpuFill8(action, 0, sizeof(PlayerActionPossibilities));
    action->zoneId = Field_GetPlayerStateZoneID(field);
    action->gsys = Field_GetGameSystem(field);
    action->field = field;
    player = Field_GetPlayer(field);
    action->exState = FieldPlayer_GetExState(player);
    actor = FieldPlayer_GetActorInFront(player);
    action->actorInFront = actor;
    if (actor != NULL) {
        objCode = FldAct_GetObjCode(actor);
        actorSystem = Field_GetActorSystem(field);
        CopyActorWPos(actor, &position);
        if (IsNPCStrengthRock(objCode) && !func_ov010_0216e5b0(actorSystem, &position)) {
            action->flags |= 8;
        }
        if (objCode == 108) {
            action->flags |= 1;
        }
    }
    direction = FieldPlayer_GetFaceDir(player);
    tileUnder = FieldPlayer_GetTileTypeUnder(player);
    tileInFront = FieldPlayer_GetTileTypeInDir(player, direction);
    if (CheckSurfHeightAllow(player, tileUnder, tileInFront) == TRUE) {
        action->flags |= 2;
    }
    if (CheckCanInteractWaterfall(player, tileUnder, tileInFront) == TRUE) {
        action->flags |= 4;
    }
    if (func_02013c40(action->zoneId) == TRUE) {
        action->flags |= 0x10;
        action->flags |= 0x40;
    }
    if (func_02013c50(action->zoneId) == TRUE) {
        action->flags |= 0x80;
    }
    if (FieldStatus_GetFlashPerms(GameData_GetFieldStatus(GSYS_GetGameData(action->gsys))) & FLD_FLASH_ALLOW) {
        action->flags |= 0x20;
    }
    if (GetAbyssalRuinsDiveZoneID(field, &diveZone) == TRUE) {
        action->flags |= 0x400;
    }
    if (IsZoneAbyssalRuinsInside(action->zoneId)) {
        FieldPlayer_GetGPos(Field_GetPlayer(action->field), &x, &y, &z);
        if (func_0201400c(action->zoneId) &&
            func_ov010_0215f118(&ABYSSAL_RUINS_FLASH_ROCK_RADIUS, x, z, 0x113, field)) {
            action->flags |= 0x20;
        }
        if (func_02014018(action->zoneId) &&
            func_ov010_0215f118(&ABYSSAL_RUINS_STRENGTH_ROCK_RADIUS, x, z, 0x112, field)) {
            action->flags |= 8;
        }
    }
    if (!func_02013e04(action->zoneId)) {
        action->flags = 0;
    } else {
        action->flags |= 0x100;
        action->flags |= 0x200;
    }
    if (GameData_IsForceSeasonSync(GSYS_GetGameData(action->gsys)) == TRUE) {
        action->flags = 0;
    }
}

u32 CheckAllowHidenEvent(u32 kind, PlayerActionPossibilities *context) {
    HiddenCheckFunc check = GetHidenEventCheckFunc(context, kind);
    if (check == NULL) {
        return TRUE;
    }
    return check(context);
}

void func_ov010_0215ea30(HiddenEventArgs *args, u16 partySlot, u16 kind, u32 value) {
    args->partySlot = partySlot;
    args->kind = kind;
    args->value = value;
}

GameEvent *CreateHidenEvent(u32 kind, HiddenEventArgs *args, PlayerActionPossibilities *context) {
    HiddenCtorFunc ctor = GetHidenEventCtorFunc(context, kind);
    if (ctor == NULL) {
        return NULL;
    }
    return ctor(args, context);
}

u32 EventCutCall_Check(PlayerActionPossibilities *context) {
    u32 result = FALSE;
    if (func_ov010_0215f104(context, 0) == 0) {
        result = TRUE;
    }
    return result;
}

GameEvent *EventCutCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context) {
    GameEvent *event;
    HiddenEventData *data;

    event = GameEvent_Create(context->gsys, NULL, EventCutCall_Callback, sizeof(HiddenEventData));
    data = GameEvent_GetData(event);
    func_ov010_0215f0e8(data, param, context);
    return event;
}

GameEventReturnCode EventCutCall_Callback(GameEvent *event, u32 *state, void *data) {
    HiddenEventData *work = data;
    u16 param;
    ScriptWork *scriptWork;

    GameData_GetEventWork(GSYS_GetGameData(work->gsys));
    param = work->args.partySlot;
    scriptWork = EventScriptCall_Replace(event, 0x2715, 0, 0);
    ScriptWork_SetParams(scriptWork, param, 0, 0, 0);
    return GAMEEVENT_CONTINUE;
}

u32 EventSurfCall_Check(PlayerActionPossibilities *context) {
    u32 state = context->exState;
    u32 result;
    if (state == 2) {
        return 4;
    }
    if (state == 3) {
        return 1;
    }
    result = TRUE;
    if (func_ov010_0215f104(context, 1) != 0) {
        result = FALSE;
    }
    return result;
}

GameEvent *EventSurfCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context) {
    GameEvent *event;
    HiddenEventData *data;

    event = GameEvent_Create(context->gsys, NULL, EventSurfCall_Callback, sizeof(HiddenEventData));
    data = GameEvent_GetData(event);
    func_ov010_0215f0e8(data, param, context);
    return event;
}

GameEventReturnCode EventSurfCall_Callback(GameEvent *event, u32 *state, void *data) {
    HiddenEventData *work = data;
    u16 param;
    ScriptWork *scriptWork;

    GameData_GetEventWork(GSYS_GetGameData(work->gsys));
    param = work->args.partySlot;
    scriptWork = EventScriptCall_Replace(event, 0x2713, 0, 0);
    ScriptWork_SetParams(scriptWork, param, 0, 0, 0);
    return GAMEEVENT_CONTINUE;
}

u32 EventWaterfallCall_Check(PlayerActionPossibilities *context) {
    switch (func_ov010_0215f104(context, 2)) {
    case FALSE:
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

GameEvent *EventWaterfallCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context) {
    GameEvent *event;
    HiddenEventData *data;

    event = GameEvent_Create(context->gsys, NULL, EventWaterfallCall_Callback, sizeof(HiddenEventData));
    data = GameEvent_GetData(event);
    func_ov010_0215f0e8(data, param, context);
    return event;
}

GameEventReturnCode EventWaterfallCall_Callback(GameEvent *event, u32 *state, void *data) {
    HiddenEventData *work = data;
    u16 param;
    ScriptWork *scriptWork;

    GameData_GetEventWork(GSYS_GetGameData(work->gsys));
    param = work->args.partySlot;
    scriptWork = EventScriptCall_Replace(event, 0x2717, 0, 0);
    ScriptWork_SetParams(scriptWork, param, 0, 0, 0);
    return GAMEEVENT_CONTINUE;
}

u32 EventStrengthCall_Check(PlayerActionPossibilities *context) {
    switch (func_ov010_0215f104(context, 3)) {
    default:
        return FALSE;
    case 0:
        return TRUE;
    }
}

GameEvent *EventStrengthCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context) {
    GameEvent *event;
    HiddenEventData *data;

    event = GameEvent_Create(context->gsys, NULL, EventStrengthCall_Callback, sizeof(HiddenEventData));
    data = GameEvent_GetData(event);
    func_ov010_0215f0e8(data, param, context);
    return event;
}

GameEventReturnCode EventStrengthCall_Callback(GameEvent *event, u32 *state, void *data) {
    HiddenEventData *work = data;
    u16 param;
    ScriptWork *scriptWork;

    GameData_GetEventWork(GSYS_GetGameData(work->gsys));
    param = work->args.partySlot;
    scriptWork = EventScriptCall_Replace(event, 0x2711, 0, 0);
    ScriptWork_SetParams(scriptWork, param, 0, 0, 0);
    return GAMEEVENT_CONTINUE;
}

u32 func_ov010_0215ec2c(PlayerActionPossibilities *context) {
    switch (func_ov010_0215f104(context, 8)) {
    default:
        return FALSE;
    case 0:
        return TRUE;
    }
}

GameEvent *func_ov010_0215ec40(HiddenEventArgs *args, PlayerActionPossibilities *context) {
    return func_ov021_021c6078(context->gsys, context->field, args->partySlot);
}

GameEvent *EventFly_Create(HiddenEventArgs *args, PlayerActionPossibilities *context) {
    func_ov010_02165534(0x13);
    return func_ov021_021c8268(context->gsys, context->field, args->value);
}

u32 EventFly_Check(PlayerActionPossibilities *context) {
    switch (func_ov010_0215f104(context, 4)) {
    case FALSE:
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

u32 EventFlash_Check(PlayerActionPossibilities *context) {
    switch (func_ov010_0215f104(context, 5)) {
    default:
        return FALSE;
    case 0:
        return TRUE;
    }
}

GameEvent *EventFlash_Create(HiddenEventArgs *param, PlayerActionPossibilities *context) {
    GameEvent *event;
    HiddenEventData *data;

    event = GameEvent_Create(context->gsys, NULL, EventFlash_Callback, sizeof(HiddenEventData));
    data = GameEvent_GetData(event);
    func_ov010_0215f0e8(data, param, context);
    return event;
}

GameEventReturnCode EventFlash_Callback(GameEvent *event, u32 *state, void *data) {
    HiddenEventData *work = data;
    GameData *gameData;
    FieldStatus *status;
    void *flash;
    HeapID heapId;
    ScriptWork *scriptWork;
    u16 param;
    u32 flags;
    FieldStatus *status2;

    gameData = GSYS_GetGameData(work->gsys);
    GameData_GetEventWork(gameData);
    status = GameData_GetFieldStatus(gameData);
    flash = FieldSkillMapEff_GetFlash(Field_GetSkillMapEff(GSYS_GetField(work->gsys)));
    heapId = Field_GetHeapID(GSYS_GetField(work->gsys));
    switch (*state) {
    case 0:
        param = work->args.partySlot;
        scriptWork = EventScriptCall_Start(event, 0x2718, NULL, 0, heapId);
        ScriptWork_SetParams(scriptWork, param, 0, 0, 0);
        ++*state;
        break;
    case 1:
        if (!FieldStatus_CheckFlashUsed(status)) {
            status2 = GameData_GetFieldStatus(gameData);
            FieldStatus_SetFlashUsed(status, TRUE);
            func_ov010_02165534(0x94);
            func_ov021_021c7e50(flash, 1);
            flags = FieldStatus_GetFlashPerms(status2);
            flags &= ~FLD_FLASH_ALLOW;
            flags |= FLD_FLASH_ACTIVE;
            FieldStatus_SetFlashPerms(status2, flags);
        } else {
            return GAMEEVENT_DONE;
        }
        ++*state;
        break;
    case 2:
        if (func_ov021_021c7e54(flash) == 3)
            return GAMEEVENT_DONE;
        break;
    }
    return GAMEEVENT_CONTINUE;
}


u32 EventDigCall_Check(PlayerActionPossibilities *context) {
    switch (func_ov010_0215f104(context, 7)) {
    case FALSE:
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

GameEvent *EventDigCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context) {
    GameEvent *event;
    HiddenEventData *data;
    func_ov010_02165534(0x5b);
    event = GameEvent_Create(context->gsys, NULL, EventDigCall_Callback, sizeof(HiddenEventData));
    data = GameEvent_GetData(event);
    func_ov010_0215f0e8(data, param, context);
    return event;
}

GameEventReturnCode EventDigCall_Callback(GameEvent *event, u32 *state, void *data) {
    HiddenEventData *work = data;
    ScriptWork *scriptWork;
    u16 param;

    switch (*state) {
    case 0:
        param = work->args.partySlot;
        scriptWork = EventScriptCall_Start(event, 0x271a, NULL, 0, 0x15);
        ScriptWork_SetParams(scriptWork, param, 0, 0, 0);
        ++*state;
        break;
    case 1:
        GameEvent_Replace(event, EventMapChangeDig_Create(work->gsys));
        break;
    }
    return GAMEEVENT_CONTINUE;
}

u32 EventTeleportCall_Check(PlayerActionPossibilities *context) {
    switch (func_ov010_0215f104(context, 6)) {
    case FALSE:
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

GameEvent *EventTeleportCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context) {
    GameEvent *event;
    HiddenEventData *data;
    func_ov010_02165534(0x64);
    event = GameEvent_Create(context->gsys, NULL, EventTeleportCall_Callback, sizeof(HiddenEventData));
    data = GameEvent_GetData(event);
    func_ov010_0215f0e8(data, param, context);
    return event;
}

GameEventReturnCode EventTeleportCall_Callback(GameEvent *event, u32 *state, void *data) {
    HiddenEventData *work = data;
    ScriptWork *scriptWork;
    u16 param;

    switch (*state) {
    case 0:
        param = work->args.partySlot;
        scriptWork = EventScriptCall_Start(event, 0x2719, NULL, 0, 0x15);
        ScriptWork_SetParams(scriptWork, param, 0, 0, 0);
        ++*state;
        break;
    case 1:
        GameEvent_Replace(event, EventMapChangeTeleport_Create(work->gsys));
        break;
    }
    return GAMEEVENT_CONTINUE;
}

u32 EventDivingCall_Check(PlayerActionPossibilities *context) {
    switch (func_ov010_0215f104(context, 10)) {
    case FALSE:
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

GameEvent *EventDivingCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context) {
    GameEvent *event;
    HiddenEventData *data;
    event = GameEvent_Create(context->gsys, NULL, EventDivingCall_Callback, sizeof(HiddenEventData));
    data = GameEvent_GetData(event);
    func_ov010_0215f0e8(data, param, context);
    return event;
}

GameEventReturnCode EventDivingCall_Callback(GameEvent *event, u32 *state, void *data) {
    HiddenEventData *work = data;
    ScriptWork *scriptWork;
    u16 param;

    GameData_GetEventWork(GSYS_GetGameData(work->gsys));
    param = work->args.partySlot;
    scriptWork = EventScriptCall_Replace(event, 0x271c, 0, 0);
    ScriptWork_SetParams(scriptWork, param, 0, 0, 0);
    return GAMEEVENT_CONTINUE;
}

u32 func_ov010_0215ef2c(PlayerActionPossibilities *context) {
    switch (func_ov010_0215f104(context, 9)) {
    default:
        return FALSE;
    case 0:
        return TRUE;
    }
}

GameEvent *func_ov010_0215ef40(HiddenEventArgs *args, PlayerActionPossibilities *context) {
    func_ov010_02165534(0x1c0);
    return func_ov021_021d1c24(context->gsys, context->field, args->partySlot);
}

GameEventReturnCode EventRuinsStrengthCall_Callback(GameEvent *event, u32 *state, void *data) {
    HiddenEventData *work = data;
    ScriptWork *scriptWork;
    u16 param;
    HeapID heapId;

    GameData_GetEventWork(GSYS_GetGameData(work->gsys));
    heapId = Field_GetHeapID(GSYS_GetField(work->gsys));
    switch (*state) {
    case 0:
        param = work->args.partySlot;
        scriptWork = EventScriptCall_Start(event, 0x271e, NULL, 0, heapId);
        ScriptWork_SetParams(scriptWork, param, 0, 0, 0);
        ++*state;
        break;
    case 1:
        EventScriptCall_Replace(event, 0x28e3, 0, 0);
        break;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *EventRuinsStrengthCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context) {
    GameEvent *event;
    HiddenEventData *data;
    event = GameEvent_Create(context->gsys, NULL, EventRuinsStrengthCall_Callback, sizeof(HiddenEventData));
    data = GameEvent_GetData(event);
    func_ov010_0215f0e8(data, param, context);
    return event;
}

GameEventReturnCode EventRuinsFlash_Callback(GameEvent *event, u32 *state, void *data) {
    HiddenEventData *work = data;
    ScriptWork *scriptWork;
    u16 param;
    HeapID heapId;

    GameData_GetEventWork(GSYS_GetGameData(work->gsys));
    heapId = Field_GetHeapID(GSYS_GetField(work->gsys));
    switch (*state) {
    case 0:
        param = work->args.partySlot;
        scriptWork = EventScriptCall_Start(event, 0x271d, NULL, 0, heapId);
        ScriptWork_SetParams(scriptWork, param, 0, 0, 0);
        ++*state;
        break;
    case 1:
        EventScriptCall_Replace(event, 0x28e2, 0, 0);
        break;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *EventRuinsFlash_Create(HiddenEventArgs *param, PlayerActionPossibilities *context) {
    GameEvent *event;
    HiddenEventData *data;
    event = GameEvent_Create(context->gsys, NULL, EventRuinsFlash_Callback, sizeof(HiddenEventData));
    data = GameEvent_GetData(event);
    func_ov010_0215f0e8(data, param, context);
    return event;
}

HiddenCheckFunc GetHidenEventCheckFunc(PlayerActionPossibilities *context, u32 kind) {
    if ((s32)kind >= 11) {
        return NULL;
    }
    if (IsZoneAbyssalRuinsInside(context->zoneId)) {
        return HIDEN_EVENTS_RUINS[kind].check;
    }
    return HIDEN_EVENTS_NORMAL[kind].check;
}

HiddenCtorFunc GetHidenEventCtorFunc(PlayerActionPossibilities *context, u32 kind) {
    if ((s32)kind >= 11) {
        return NULL;
    }
    if (IsZoneAbyssalRuinsInside(context->zoneId)) {
        return HIDEN_EVENTS_RUINS[kind].create;
    }
    return HIDEN_EVENTS_NORMAL[kind].create;
}

void func_ov010_0215f0e8(HiddenEventData *data, HiddenEventArgs *param, PlayerActionPossibilities *context) {
    data->magic = 0x19740205;
    data->actor = context->actorInFront;
    data->args = *param;
    data->gsys = context->gsys;
}

u32 func_ov010_0215f104(PlayerActionPossibilities *context, u32 value) {
    u16 flags = context->flags;
    u32 mask = 1 << value;
    return (flags & mask) ? TRUE : FALSE;
}

BOOL func_ov010_0215f118(const HiddenArea *area, u16 x, u16 z, u16 flag, Field *field) {
    GameSystem *gsys;
    GameData *gameData;
    EventWork *eventWork;
    s32 dx;
    s32 dz;

    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    eventWork = GameData_GetEventWork(gameData);
    if (EventWork_FlagGet(eventWork, flag)) {
        return FALSE;
    }
    dx = area->x - x;
    dz = area->z - z;
    if (dx < 0) {
        dx = -dx;
    }
    if (dx < area->width) {
        if (dz < 0) {
            dz = -dz;
        }
        if (dz < area->height) {
            return TRUE;
        }
    }
    return FALSE;
}
