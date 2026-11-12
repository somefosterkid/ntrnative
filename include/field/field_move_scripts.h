// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_move_scripts.h: the prototypes
// of Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_MOVE_SCRIPTS_H
#define BWPORT_FIELD_FIELD_MOVE_SCRIPTS_H

#include "types.h"
#include "struct_decls.h"

BOOL ScriptNative_SurfTCBWait(VM *vm, void *env);

BOOL s00C5_CallSurf(VM *vm, FieldScriptEnv *env);
BOOL s00C6_CallWaterfall(VM *vm, FieldScriptEnv *env);
BOOL s00C7_CallCut(VM *vm, FieldScriptEnv *env);
BOOL s00C8_CallDiving(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL ScriptNative_WaterfallTCBWait(VM *vm, void *env);

#endif // BWPORT_FIELD_FIELD_MOVE_SCRIPTS_H
