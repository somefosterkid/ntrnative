// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_mapchange.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_SCRCMD_MAPCHANGE_H
#define BWPORT_FIELD_SCRCMD_MAPCHANGE_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

BOOL s00C2_MapChangeWarp(VM *vm, FieldScriptEnv *env);
BOOL s00BE_MapChangeFake(VM *vm, FieldScriptEnv *env);
BOOL s00C1_MapChangeQuicksand(VM *vm, FieldScriptEnv *env);
BOOL s00C3_MapChangeUnionRoom(VM *vm, FieldScriptEnv *env);
BOOL s00C4_MapChangeCore(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s00BF_MapChangeWarpPad(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_MAPCHANGE_H
