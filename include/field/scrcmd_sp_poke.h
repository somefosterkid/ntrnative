// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_sp_poke.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_SCRCMD_SP_POKE_H
#define BWPORT_FIELD_SCRCMD_SP_POKE_H

#include "types.h"
#include "struct_decls.h"

// Script commands of the Nuvema Town gimmick: one sets its flag, s023C_NuvemaGimmickPlay, s023D_NuvemaGimmickWaitAnim
BOOL func_ov010_0216f8b8(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216f8c8(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216f8dc(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_SP_POKE_H
