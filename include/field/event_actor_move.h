// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_actor_move.h, the lines in use
// so far.
#ifndef BWPORT_FIELD_EVENT_ACTOR_MOVE_H
#define BWPORT_FIELD_EVENT_ACTOR_MOVE_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "system/game_event.h"

struct EventActorJumpWork {
    void *effects;
    FieldActor *actor;
    s16 frame;
    u16 pad0A;
    VecFx32 start;
    VecFx32 displacement;
};

// The height of a jump at each frame
extern const fx32 JUMP_ANY_HEIGHT_TABLE[16];

GameEventReturnCode EventActorJump_Callback(GameEvent *event, u32 *state, void *data);
GameEvent *EventActorJump_Create(GameSystem *gsys, FieldActor *actor, const VecFx32 *start, const VecFx32 *end);

#endif // BWPORT_FIELD_EVENT_ACTOR_MOVE_H
