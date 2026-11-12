// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_trainer_eye.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_EVENT_TRAINER_EYE_H
#define BWPORT_FIELD_EVENT_TRAINER_EYE_H

#include "types.h"
#include "field/trainer_script.h"
#include "gfl/heap.h"
#include "struct_decls.h"

typedef struct EventTrainerEyeWork EventTrainerEyeWork;

// The event that walks one or two Trainers up to the player
GameEvent *EventTrainerEye_Create(GameSystem *gsys, EventTrainerEyeWork *eye1, EventTrainerEyeWork *eye2);

// How far the actor, facing dir, sees the player within range, or -1 if it doesn't
int func_ov021_021ae198(FieldActor *actor, u16 dir, int range, EncountSystem *encounter);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
EventTrainerEyeWork *EventTrainerEye_CreateData(HeapID heapId, Field *field, const TrainerClashData *data,
                                                BOOL isPair);
// The Trainer ID of a Trainer actor
u16 GetNPCTrainerID(FieldActor *actor);
// The event of the Trainers that have seen the player, or NULL if none has
GameEvent *EventTrainerEye_CheckAll(Field *field);

#endif // BWPORT_FIELD_EVENT_TRAINER_EYE_H
