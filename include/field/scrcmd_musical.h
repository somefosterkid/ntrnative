// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_musical.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_SCRCMD_MUSICAL_H
#define BWPORT_FIELD_SCRCMD_MUSICAL_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

BOOL func_ov010_0215d864(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215dadc(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215e1c0(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s005D_WordSetMusicalInfo(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215d34c(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215d468(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215d504(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215d7d4(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_MUSICAL_H
