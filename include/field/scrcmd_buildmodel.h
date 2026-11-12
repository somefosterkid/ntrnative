// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_buildmodel.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_SCRCMD_BUILDMODEL_H
#define BWPORT_FIELD_SCRCMD_BUILDMODEL_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

BOOL s0125_BMPlayHOFMachineSeq(VM *vm, FieldScriptEnv *env);
BOOL s012F_PokecenPlayHealingSequence(VM *vm, FieldScriptEnv *env);
BOOL s0130_PokecenPCOpen(VM *vm, FieldScriptEnv *env);
BOOL s0132_PokecenPCClose(VM *vm, FieldScriptEnv *env);
BOOL s012B_BMAnmPlayInv(VM *vm, FieldScriptEnv *env);
BOOL s012E_BMAnmPlayLoop(VM *vm, FieldScriptEnv *env);
BOOL s012D_BMSetVisible(VM *vm, FieldScriptEnv *env);
BOOL s0126_BMChangeMdlID(VM *vm, FieldScriptEnv *env);
BOOL s0127_BMCreateHandleByGPos(VM *vm, FieldScriptEnv *env);
BOOL s0128_BMReleaseHandle(VM *vm, FieldScriptEnv *env);
BOOL s0129_BMHndAudioVisualAnmPlay(VM *vm, FieldScriptEnv *env);
BOOL s0124_BMHndAnmPlay(VM *vm, FieldScriptEnv *env);
BOOL s012C_BMHndAnmPause(VM *vm, FieldScriptEnv *env);
BOOL s012A_BMHndAnmWait(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s0131_PokecenPCIdle(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_BUILDMODEL_H
