// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_encount.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_SCRCMD_ENCOUNT_H
#define BWPORT_FIELD_SCRCMD_ENCOUNT_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

BOOL s0175_CallWildBattleEnd(VM *vm, FieldScriptEnv *env);
BOOL s0179_CallCaptureDemo(VM *vm, FieldScriptEnv *env);
BOOL s00BC_PhenomenonGetItemID(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b5154(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b50d0(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b51bc(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s0174_CallWildBattle(VM *vm, FieldScriptEnv *env);
BOOL s0297_CallWildBattleEx(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_ENCOUNT_H
