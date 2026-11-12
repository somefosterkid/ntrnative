// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/game_event.h, the lines in use so
// far.
#ifndef BWPORT_SYSTEM_GAME_EVENT_H
#define BWPORT_SYSTEM_GAME_EVENT_H

// Names, layouts and constants from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

typedef enum {
    GAMEEVENT_CONTINUE = 0x0,
    GAMEEVENT_DONE = 0x1,
    // Runs the current event again in the same frame, such as an event that was just chained
    GAMEEVENT_CONTINUE_DIRECT = 0x21,
} GameEventReturnCode;

typedef GameEventReturnCode (*GameEventCallback)(GameEvent *event, u32 *state, void *data);
// Creates an event for GameEvent_CreateOverlayDelegate, after it loads the event's overlay
typedef GameEvent *(*GameEventProvider)(GameSystem *gsys, void *args);

void GameEvent_ChainNext(GameEvent *event, GameEvent *next);
GameEvent *GameEvent_Create(GameSystem *gsys, GameEvent *parent, GameEventCallback callback, u32 size);
void *GameEvent_GetData(GameEvent *event);
GameEvent *GameEvent_CreateOverlayDelegate(GameSystem *gsys, u32 overlayId, GameEventProvider provider, void *args);
GameSystem *GameEvent_GetGameSystem(GameEvent *event);
void GameEvent_Replace(GameEvent *event, GameEvent *next);

// pokebw2's EventFieldSubprocessTransition_CreateWithCallback
// Runs a proc of an overlay, then calls onEnd, if it is given, with arg
GameEvent *func_02014808(GameSystem *gsys, Field *field, u32 overlayId, const GameProcFunctions *functions, void *param,
                         void (*onEnd)(void *arg), void *arg);

// Runs the current event, and the ones it returns to while they ask for it. Whether the last event ended
BOOL GSYS_UpdateEvents(GameSystem *gsys);
// Starts the event that provider creates, if it creates one and no event is running
BOOL GSYS_FeedEventsFromProvider(GameSystem *gsys, GameEventProvider provider, void *data);

// Gives the event a new callback and new data of size bytes
void GameEvent_Transplant(GameEvent *event, GameEventCallback callback, u32 size);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 *GameEvent_GetStatePtr(GameEvent *event);
// The event that goes on when this one is done, or NULL
GameEvent *GameEvent_GetReturnEvent(GameEvent *event);

#endif // BWPORT_SYSTEM_GAME_EVENT_H
