#ifndef BWPORT_FIELD_FLDMMDL_MOVECODE_PAIR_H
#define BWPORT_FIELD_FLDMMDL_MOVECODE_PAIR_H

// fldmmdl_movecode_pair.c: the move codes that follow another actor or the player, hide an actor under a field
// effect, turn it and walk it along walls

#include "types.h"
#include "struct_decls.h"
#include "field/field_effect.h"

// The field effect that the actor hides in (pokebw2's FldmmdlMove_SetHideEffect / GetHideEffect), and the setting
// of the flag that it has none (FldmmdlMove_SetHideNoEffect)
void func_ov021_0219bf3c(FieldActor *actor, FieldEffectTask *task);
FieldEffectTask *func_ov021_0219bf48(FieldActor *actor);
void func_ov021_0219bf54(FieldActor *actor);

#endif // BWPORT_FIELD_FLDMMDL_MOVECODE_PAIR_H
