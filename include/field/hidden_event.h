// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/hidden_event.h, the lines in use so
// far. pokebw2's func_ov012_02159418 is Black's func_ov010_0215ea30.
#ifndef BWPORT_FIELD_HIDDEN_EVENT_H
#define BWPORT_FIELD_HIDDEN_EVENT_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "system/game_event.h"

// What the field menu or a shortcut asks of a hidden move
struct HiddenEventArgs {
    u16 partySlot;
    u16 kind;
    // Fly's destination
    u32 value;
};

void func_ov010_0215ea30(HiddenEventArgs *args, u16 partySlot, u16 kind, u32 value);
GameEvent *CreateHidenEvent(u32 kind, HiddenEventArgs *args, PlayerActionPossibilities *context);

// Black 2's, not checked against Black
typedef u32 (*HiddenCheckFunc)(PlayerActionPossibilities *context);

// Black 2's, not checked against Black
typedef GameEvent *(*HiddenCtorFunc)(HiddenEventArgs *args, PlayerActionPossibilities *context);

HiddenCheckFunc GetHidenEventCheckFunc(PlayerActionPossibilities *context, u32 kind);
GameEventReturnCode EventDigCall_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventTeleportCall_Callback(GameEvent *event, u32 *state, void *data);

struct HiddenArea {
    u16 x;
    u16 z;
    u16 width;
    u16 height;
};

// The data of the events that call a hidden move's script
struct HiddenEventData {
    // 0x19740205
    u32 magic;
    FieldActor *actor;
    GameSystem *gsys;
    HiddenEventArgs args;
};

// Black 2's, not checked against Black
struct HiddenEventSpec {
    HiddenCtorFunc create;
    HiddenCheckFunc check;
};

u32 CheckAllowHidenEvent(u32 kind, PlayerActionPossibilities *context);
u32 EventCutCall_Check(PlayerActionPossibilities *context);
GameEventReturnCode EventCutCall_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventSurfCall_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventWaterfallCall_Callback(GameEvent *event, u32 *state, void *data);
u32 EventStrengthCall_Check(PlayerActionPossibilities *context);
GameEventReturnCode EventStrengthCall_Callback(GameEvent *event, u32 *state, void *data);
GameEvent *EventFly_Create(HiddenEventArgs *args, PlayerActionPossibilities *context);
u32 EventFlash_Check(PlayerActionPossibilities *context);
GameEventReturnCode EventFlash_Callback(GameEvent *event, u32 *state, void *data);
GameEvent *EventDigCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context);
GameEvent *EventTeleportCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context);
GameEvent *EventDivingCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context);
GameEventReturnCode EventDivingCall_Callback(GameEvent *event, u32 *state, void *data);
u32 func_ov010_0215ec2c(PlayerActionPossibilities *context);
GameEvent *func_ov010_0215ec40(HiddenEventArgs *args, PlayerActionPossibilities *context);
u32 func_ov010_0215ef2c(PlayerActionPossibilities *context);
GameEvent *func_ov010_0215ef40(HiddenEventArgs *args, PlayerActionPossibilities *context);
void func_ov010_0215f0e8(HiddenEventData *data, HiddenEventArgs *param, PlayerActionPossibilities *context);
u32 func_ov010_0215f104(PlayerActionPossibilities *context, u32 value);
BOOL func_ov010_0215f118(const HiddenArea *area, u16 x, u16 z, u16 flag, Field *field);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
HiddenCtorFunc GetHidenEventCtorFunc(PlayerActionPossibilities *context, u32 kind);
GameEvent *EventCutCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context);
u32 EventSurfCall_Check(PlayerActionPossibilities *context);
GameEvent *EventSurfCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context);
u32 EventWaterfallCall_Check(PlayerActionPossibilities *context);
GameEvent *EventWaterfallCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context);
GameEvent *EventStrengthCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context);
u32 EventFly_Check(PlayerActionPossibilities *context);
GameEvent *EventFlash_Create(HiddenEventArgs *param, PlayerActionPossibilities *context);
u32 EventDigCall_Check(PlayerActionPossibilities *context);
u32 EventTeleportCall_Check(PlayerActionPossibilities *context);
u32 EventDivingCall_Check(PlayerActionPossibilities *context);
GameEventReturnCode EventRuinsStrengthCall_Callback(GameEvent *event, u32 *state, void *data);
GameEvent *EventRuinsStrengthCall_Create(HiddenEventArgs *param, PlayerActionPossibilities *context);
GameEventReturnCode EventRuinsFlash_Callback(GameEvent *event, u32 *state, void *data);
GameEvent *EventRuinsFlash_Create(HiddenEventArgs *param, PlayerActionPossibilities *context);

#endif // BWPORT_FIELD_HIDDEN_EVENT_H
