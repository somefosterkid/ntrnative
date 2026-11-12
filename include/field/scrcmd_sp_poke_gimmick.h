// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_sp_poke_gimmick.h: the lines in
// use so far.
#ifndef BWPORT_FIELD_SCRCMD_SP_POKE_GIMMICK_H
#define BWPORT_FIELD_SCRCMD_SP_POKE_GIMMICK_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

BOOL func_ov010_0216bbd4(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216bc88(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216bce4(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216bf38(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s020D_PokePartyFindEx(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216bd1c(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216bec0(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_SP_POKE_GIMMICK_H
