// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_trainer.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_SCRCMD_TRAINER_H
#define BWPORT_FIELD_SCRCMD_TRAINER_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

BOOL s0080_TrainerEyeEventInit(VM *vm, FieldScriptEnv *env);
BOOL s0081_TrainerEyeEventStart(VM *vm, FieldScriptEnv *env);
BOOL s0083_TrainerGetBattleType(VM *vm, FieldScriptEnv *env);
BOOL s007F_TrainerEyeGetTrainerID(VM *vm, FieldScriptEnv *env);
BOOL s0082_TrainerGetActorID(VM *vm, FieldScriptEnv *env);
BOOL s0084_ActorGetTrainerID(VM *vm, FieldScriptEnv *env);
BOOL s0085_CallTrainerBattle(VM *vm, FieldScriptEnv *env);
BOOL s0085_CallTrainerMultiBattle(VM *vm, FieldScriptEnv *env);
BOOL s0088_TrainerGetMessageTypes(VM *vm, FieldScriptEnv *env);
BOOL s008B_TrainerBGMPlayPush(VM *vm, FieldScriptEnv *env);
BOOL s0092_TrainerGetFieldAction(VM *vm, FieldScriptEnv *env);
BOOL s0093_TrainerGetPrizeItem(VM *vm, FieldScriptEnv *env);
BOOL s0094_CallTradedPokemonBattle(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s008A_TrainerGetBattleType(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_TRAINER_H
