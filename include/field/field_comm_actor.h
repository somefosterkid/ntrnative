// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_comm_actor.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_COMM_ACTOR_H
#define BWPORT_FIELD_FIELD_COMM_ACTOR_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

// Twins of pokebw2's field_comm_actor.c; the parameters as Black's code has them
FldCommActSys *FldCommActSys_Create(u8 a0, MMSys *actorSystem, HeapID heapId, int a3);
void FldCommActSys_CreateActor(FldCommActSys *sys, u32 netId, u16 a2, const u16 *dir, const VecFx32 *pos,
                               const u32 *a5);
void FldCommActSys_DeleteActor(FldCommActSys *sys, u32 netId);
u32 FldCommActSys_FindActor(FldCommActSys *sys, s16 a1, s32 a2, s16 a3, u32 *a4, u32 *a5);

// One player's actor (pokebw2's FldCommActEntry)
typedef struct FldCommActEntry {
    u32 netId;
    u16 actorDeleted;
    u16 unk6;
    FieldActor *actor;
} FldCommActEntry;

struct FldCommActSys {
    u16 count;
    u16 unk2;
    MMSys *actorSystem;
    FldCommActEntry *entries;
};

// What an actor's position block holds while it is another player's actor (pokebw2's FldCommActLink)
typedef struct FldCommActLink {
    const u16 *dir;
    const VecFx32 *pos;
    const u32 *hidden;
    FldCommActEntry *entry;
} FldCommActLink;

#endif // BWPORT_FIELD_FIELD_COMM_ACTOR_H
