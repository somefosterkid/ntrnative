// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_actor_move.h, with Black's
// names.
#ifndef BWPORT_FIELD_SCRCMD_ACTOR_MOVE_H
#define BWPORT_FIELD_SCRCMD_ACTOR_MOVE_H

// Overlay 10's scrcmd_actor_move.c (pokebw2's descriptive name): the script commands that raise or lower an actor
// over frames and circle it around its place

#include "types.h"
#include "struct_decls.h"

BOOL func_ov010_0216a7a0(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216a884(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_ACTOR_MOVE_H
