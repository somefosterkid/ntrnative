// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_trial_house.h: the lines in use
// so far.
#ifndef BWPORT_FIELD_SCRCMD_TRIAL_HOUSE_H
#define BWPORT_FIELD_SCRCMD_TRIAL_HOUSE_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

BOOL s01E6_TrialHouseWorkInit(VM *vm, FieldScriptEnv *env);
BOOL s01E7_TrialHouseWorkDelete(VM *vm, FieldScriptEnv *env);
BOOL s01E8_TrialHousePrepareParty(VM *vm, FieldScriptEnv *env);
BOOL s01E9_TrialHouseCallTeamSelect(VM *vm, FieldScriptEnv *env);
BOOL s01EB_TrialHouseMsgDisp(VM *vm, FieldScriptEnv *env);
BOOL s01EC_TrialHouseStartBattle(VM *vm, FieldScriptEnv *env);
BOOL s01EF_TrialHouseGetBattleTestRank(VM *vm, FieldScriptEnv *env);
BOOL s01F1_TrialHouseCalcPointsStars(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216a4cc(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216a5c4(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216a634(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216a6b4(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216a72c(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216a774(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s01F3_TrialHouseSaveData(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216a594(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_TRIAL_HOUSE_H
