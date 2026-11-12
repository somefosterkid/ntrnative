// After pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/union_actor.h.
#ifndef BWPORT_FIELD_UNION_ACTOR_H
#define BWPORT_FIELD_UNION_ACTOR_H

#include "types.h"
#include "struct_decls.h"

// union_actor.c, overlay 19 (the name is pokebw2's guess, the ROM has no string of it): the actors of the Union Room's
// players heard, which union_oneself.c asks about by their id (the entry's index in the high byte, the member in the
// low one)

// A step of an actor, for the player heard that it stands for
typedef BOOL (*UnionActorFunc)(UnionSystem *unisys, UnionActorSlot *slot, Field *field, u8 *seq);

// pokebw2's UnionOneself_ClearActors
void func_ov019_0217ae20(UnionSystem *unisys);
// pokebw2's UnionOneself_IsMemberId
BOOL func_ov019_0217aee8(u16 id);
// pokebw2's UnionOneself_GetEntryIndex
u16 func_ov019_0217aefc(u16 id);
// pokebw2's UnionOneself_GetActorGender
u8 func_ov019_0217af04(UnionSystem *unisys, u16 id);
// pokebw2's UnionActor_IsId
BOOL func_ov019_0217af38(u16 id);
// pokebw2's UnionOneself_CanTalkTo
BOOL func_ov019_0217af4c(UnionSystem *unisys, u16 id);
// pokebw2's UnionOneself_UpdateActors
void func_ov019_0217b264(UnionSystem *unisys, GameData *gameData, Field *field);
// pokebw2's UnionOneself_UpdateMarks
void func_ov019_0217b440(UnionSystem *unisys, GameData *gameData, Field *field);
// pokebw2's UnionActor_StepAppear
BOOL func_ov019_0217b5e0(UnionSystem *unisys, UnionActorSlot *slot, Field *field, u8 *seq);

#endif // BWPORT_FIELD_UNION_ACTOR_H
