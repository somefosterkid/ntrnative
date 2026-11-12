// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_entree_forest.h: the lines in
// use so far.
#ifndef BWPORT_FIELD_SCRCMD_ENTREE_FOREST_H
#define BWPORT_FIELD_SCRCMD_ENTREE_FOREST_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

BOOL s021C_EntreeForestStartBattle(VM *vm, FieldScriptEnv *env);
BOOL s021A_WordSetLoadEntreeForestPkmName(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s021B_EntreeForestSpawnAllPkm(VM *vm, FieldScriptEnv *env);
BOOL s0217_MapChangeEntreeForest(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216ac90(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216accc(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216ad70(VM *vm, FieldScriptEnv *env);
GameEvent *EventEntreeForestWarp_CreateToAnother(GameSystem *gsys, u32 dir, u32 area);

#endif // BWPORT_FIELD_SCRCMD_ENTREE_FOREST_H
