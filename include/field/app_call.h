// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/app_call.h, the lines in use so far.
#ifndef BWPORT_FIELD_APP_CALL_H
#define BWPORT_FIELD_APP_CALL_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "field/itemuse_event.h"
#include "field/player_action.h"

// Called with the app call's input and its arg
typedef BOOL (*FieldAppCallPredicate)(FieldAppCallInput *input, void *arg);

// Black's has no screenId after parent: Black 2's keeps the field menu's subscreen there, to go back to
struct FieldAppCallInput {
    GameSystem *gameSystem;
    Field *field;
    GameEvent *parent;
    // The app to open, from the field menu's table, or -1
    s32 appId;
    // A parameter for the app, or -1
    s32 appParam;
    FieldAppCallPredicate canRetry;
    FieldAppCallPredicate callback1;
    FieldAppCallPredicate callback2;
    void *arg;
    // What the app chose, for the caller to act on: the event type and its parameters
    u32 eventType;
    u32 eventId;
    u32 partySlot;
    u32 eventValue;
    u32 unk34;
    u32 unk38;
    u32 unk3C;
};

// The retry check a caller hands to the app call, with the input and arg it passes on (fields as Black's offsets)
typedef struct FieldAppCallParam {
    FieldAppCallPredicate canRetry;
    FieldAppCallPredicate callback1;
    FieldAppCallPredicate callback2;
    void *arg;
    FieldAppCallInput *input;
} FieldAppCallParam;

// The app call's work: the input at 0x18, the other fields by offset
typedef struct FieldAppCallWork {
    u16 code;
    u16 unk2;
    u32 appId;
    u32 prevAppId;
    u32 nextAppId;
    u32 result;
    GameEvent *event;
    FieldAppCallInput *input;
    // The open app's parameters, made by its createParam
    void *appParam;
    FieldAppCallParam params;
    PlayerActionPerms perms;
    PlayerActionPossibilities action;
    u8 unk60;
    u8 unk61;
    u16 unk62;
    u32 unk64;
    u32 unk68;
    u32 unk6C[2];
} FieldAppCallWork;

// An app the field opens: its overlay and proc, how to make its parameters from the previous app's, and how to read
// its result (pokebw2's FieldProcLink)
typedef struct FieldProcLink {
    s32 overlayId;
    const GameProcFunctions *functions;
    void *(*createParam)(FieldAppCallWork *work, s32 appParam, s32 prevAppId, void *prevParam);
    u32 (*getResult)(FieldAppCallWork *work, void *param);
    void (*call)(FieldAppCallWork *work, s32 appParam);
    void (*freeParam)(void *param);
} FieldProcLink;

#define FIELD_APP_POKELIST 0
#define FIELD_APP_BAG 2
#define FIELD_APP_POKESTATUS 6 // Black's
#define FIELD_APP_RESULT_NEXT 4

GameEvent *EventFieldAppCall_Create(FieldAppCallInput *input, u16 code);

#endif // BWPORT_FIELD_APP_CALL_H
